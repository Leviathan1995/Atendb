-module(biu_server).
-import(biu,[insert/2]).
-export([start_server/0]).

start_server() ->
	{ok, Listen} = gen_tcp:listen(1320,
			 [binary, {packet, 4},{reuseaddr, true}, {active, true}]),
	spawn(fun() -> par_connect(Listen) end).

par_connect(Listen) ->
	{ok,Socket}=gen_tcp:accept(Listen),
	spawn(fun() -> par_connect(Listen) end),
	loop(Socket).

loop(Socket) ->
    receive
	{tcp, Socket, Request} ->
	    Lstr = binary_to_term(Request),
	    Tstr=list_to_tuple(Lstr),
	    case Tstr of
		{insert,Key,Value} ->
			Response=biu:insert(Key,Value)
	    end,
	    gen_tcp:send(Socket, term_to_binary(Response)), 
	    loop(Socket);
	{tcp_closed, Socket} ->
	    io:format("Server socket closed~n")
    end.
