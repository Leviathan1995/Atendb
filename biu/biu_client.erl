-module(biu_client).
-export([biu_insert/1]).

biu_client() ->
    {ok, Socket} = 
	gen_tcp:connect("localhost", 2344,
			[binary, {packet, 4}]),
    Socket.

biu_insert(Key,Value)->
    Socket=biu_client(),
    ok = gen_tcp:send(Socket, term_to_binary(Str)),
    receive
	{tcp,Socket,Bin} ->
	    io:format("Client received binary = ~p~n",[Bin]),
	    Val = binary_to_term(Bin),
	    io:format("Client result = ~p~n",[Val]),
	    gen_tcp:close(Socket)
    end.
