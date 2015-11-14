<h1><b>LminiSQL</b></h1>

PS:B+树索引有bug，没有push，重构中



            设计并实现了一个简单的SQL引擎，用户可以通过控制台输入命令操作MiniSQL.允许用户通过SQL
            语句实现表的建立和删除， 索引的建立和删除，数据表中记录的插入和删除，查找。
   
   
<h2>SQL查询语言：</h2>
            Create table:
                        创建数据表
                        Create table r
                                    (A1 D1,
 			                         A2 D2,
                                    ... ,
                                    An Dn);
            Insert into:
                        插入记录
                        Insert into r Values
                                    (...),
                                    (...),
                                    (...);
            Select:
                        选择记录
                        Select * from table;
                        Select * from table where attributes op value;

            Create index:
                        创建索引
                        Create index index_name on table_name(column_name);
