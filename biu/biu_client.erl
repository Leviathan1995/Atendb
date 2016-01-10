-module(biu_client).
-export([biu_connect/0,biu_start/0,biu_insert/2,biu_update/2,biu_delete/1,biu_read/1,biu_stop/0]).

biu_client() ->
    {ok, Socket} = 
	gen_tcp:connect("localhost", 1343,
			[binary, {packet, 4}]),
    Socket.

biu_connect() ->
    Socket=biu_client(),
    ok=gen_tcp:send(Socket,term_to_binary([connect])),
    response(Socket).

biu_start() ->
    Socket=biu_client(),
    ok=gen_tcp:send(Socket,term_to_binary([start])),
    response(Socket).

biu_insert(Key,Value) ->
    Socket=biu_client(),
    ok=gen_tcp:send(Socket,term_to_binary([insert,Key,Value])),
    response(Socket).

biu_update(Key,Value) ->
    Socket=biu_client(),
    ok=gen_tcp:send(Socket,term_to_binary([update,Key,Value])),
    response(Socket).

biu_delete(Key) ->
    Socket=biu_client(),
    ok=gen_tcp:send(Socket,term_to_binary([delete,Key])),
    response(Socket).

biu_read(Key) ->
    Socket=biu_client(),
    ok=gen_tcp:send(Socket,term_to_binary([read,Key])),
    response(Socket).

biu_stop() ->
    Socket=biu_client(),
    ok=gen_tcp:send(Socket,term_to_binary([stop])),
    response(Socket).

response(Socket)->
    receive
	{tcp,Socket,Bin} ->
	    Val = binary_to_term(Bin),
	    io:format("~s",[Val]),
        io:format("~n"),
	    gen_tcp:close(Socket)
    end.
