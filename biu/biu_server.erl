-module(biu_server).
-export([start_server/0]).

start_server() ->
	{ok, Listen} = gen_tcp:listen(1318,
			 [binary, {packet, 4},{reuseaddr, true}, {active, true}]),
	spawn(fun() -> par_connect(Listen) end).

par_connect(Listen) ->
	{ok,Socket}=gen_tcp:accept(Listen),
	spawn(fun() -> par_connect(Listen) end),
	loop(Socket).

loop(Socket) ->
    receive
	{tcp, Socket, Response} ->
	    Lstr = binary_to_term(Response),
	    io:format("Server (unpacked)  ~p~n",[Lstr]),
	    Tstr=list_to_tuple(Lstr),
	    case Tstr of
		{insert,Key,Value} ->
			io:format("shq")
	    end,
	    gen_tcp:send(Socket, term_to_binary(Lstr)), 
	    loop(Socket);
	{tcp_closed, Socket} ->
	    io:format("Server socket closed~n")
    end.
