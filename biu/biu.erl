-module(biu).
-export([start/0, stop/0, init/1]).
-export([insert/2,delete/1,update/2,read/1]).

start() ->
    spawn(?MODULE, init, ["./biustorge"]),
    "hello".
stop() ->
    biulib ! stop,
    "bye bye".

insert(Key,Value) ->
    call_port({insert,Key,Value}).

delete(Key) ->
    call_port({delete,Key}).

update(Key,Value) ->
    call_port({update,Key,Value}).

read(Key) ->
    call_port({read,Key}).

call_port(Msg) ->
    biulib ! {call, self(), Msg},
    receive
	{biulib, Result} ->
	    Result
    end.

init(Biu) ->
    register(biulib, self()),
    process_flag(trap_exit, true),
    Port = open_port({spawn, Biu}, [{packet, 2}]),
    loop(Port).

loop(Port) ->
    receive
	{call, Caller, Msg} ->
	    Port ! {self(), {command,encode(Msg)}},
	    receive
		{Port, {data, Data}} ->
		    Caller ! {biulib, Data}
	    end,
	    loop(Port);
	stop ->
	    Port ! {self(), close},
	    receive
		{Port, closed} ->
		    exit(normal)
	    end;
	{'EXIT', Port, Reason} ->
	    exit({port_terminated,Reason})
    end.

encode({insert,Key,Value}) -> [1]++[Key]++["@"]++[Value];
encode({update,Key,Value}) -> [2]++[Key]++["@"]++[Value];
encode({delete,Key}) -> [3]++[Key];
encode({read,Key}) -> [4]++[Key].
