#The Biu
   Biu是基于Bitcask模型的存储引擎。Bitcask是一个日志型的基于hash表结构和key-value存储模型.
   
##Bitcask模型
* 日志型的数据文件:即所有写操作只追加而不修改旧的数据.
* 基于hash表的索引数据: 在Bitcask模型中,除了存储在磁盘上的数据文件,还有另外一块数据,那就是存储在内存中的hash表,hash表的作用是通过key值快速的定位到value的位置.
* Hint file:Hint中每一项的数据结构,与数据文件中的数据结构非常相似,不同的是它并不存储具体的value值,而是存储value的位置.每次引擎重启时,遍历Hint加载索引至内存中.

##Biu使用
#####Biu为C/S结构,首先启动Biu服务端
	./bius
#####启动客户端
	./client
#####通过客户端进行数据的操作
	insert(key,value)
	update(key,value)
	delete(key)
	read(key)
	
##Biu接口
* insert(key,value):插入数据
* update(key,value):更新数据
* delete(key):删除数据
* read(key):终端读取显示数据

##License:
  MIT





