-module(biu_server).
-import(biu,[insert/2,update/2,delete/1,read/1,stop/0,start/0]).
-export([start_server/0,loop/2]).

start_server() ->
	{ok, Listen} = gen_tcp:listen(1343,
			 [binary, {packet, 4},{reuseaddr, true}, {active, true}]),
	par_connect(Listen).

par_connect(Listen) ->
	{ok,Socket}=gen_tcp:accept(Listen),
	Listen_Pid=self(),
	Pid=spawn(?MODULE,loop,[Socket,Listen_Pid]),
	gen_tcp:controlling_process(Socket,Pid),
	par_connect(Listen).

loop(Socket,Listen_Pid) ->
    receive
	{tcp, Socket, Request} ->
	    Lstr = binary_to_term(Request),
	    Tstr=list_to_tuple(Lstr),
	    case Tstr of
	    	{connect} ->
	    		Response=biu:connect();
	    	{start} ->
			Response=biu:start();
	    	{insert,Key,Value} ->
			Response=biu:insert(Key,Value);
 	    	{update,Key,Value} ->
			Response=biu:update(Key,Value);
	    	{delete,Key} ->
			Response=biu:delete(Key);
	    	{read,Key} ->
			Response=biu:read(Key);
		{stop} ->
			biu:stop(),
			Response="Server cloesd ! ",
			exit(Listen_Pid,"Listen socket closed!")
	    end,
	    gen_tcp:send(Socket, term_to_binary(Response)),
	    loop(Socket,Listen_Pid);
	{tcp_closed, Socket} ->
	    io:format("Server socket closed~n")
    end.

