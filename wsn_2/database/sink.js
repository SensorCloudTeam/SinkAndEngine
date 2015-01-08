var sqlmanager = require('./sqlmanager.js');
var TABLE = 'sink';
var NODE = {};
var connection = sqlmanager.connection;
var DATABASE = sqlmanager.DATABASE;
/*ph是这样的对象[uid:,nid:,sid:,value:,time]
*uid表示用户id,nid表示节点id,sid表示传感器id,value是传感器检测的数据值,time表示检测数据的时间*/
NODE.insert = function(node,func){
	console.log('begin insert node');
	connection.query('use '+DATABASE);
	connection.query('INSERT INTO '+TABLE+' '+ 'SET id = ?,user_id = ?, name = ? ;', 
		[node.id,node.user_id,node.name] ,
		function(err,rows,fields){
			console.log('end insert node');
			if(err)
				console.log('node insert:'+err);
			if(rows&&rows.length > 0){
				if(func)
					func(true);
			}
			else{
				if(func)
					func(false);
			}
		});
};

NODE.clear = function(){
	connection.query('use '+DATABASE);
	connection.query('truncate table '+ TABLE + ';');
};

NODE.select = function(node,func){
	console.log('begin select node');
	connection.query('use '+DATABASE);
	connection.query('select * from '+TABLE+ ' where id = ? ;',
		[node.id],
		function(err,rows,fields){
			console.log('end select node');
			if(err){
				console.log('node select :'+err);
			}
			if(rows&&rows.length > 0){
				if(func)
					func(true);
			}
			else{
				if(func)
					func(false);
			}

		});
}

NODE.checkId = function(id,func){
	console.log('begin checkid');
	connection.query('use '+DATABASE);
	connection.query('select * from '+TABLE+ ' where id = ? ;',
		id,function(err,rows,fields){
			console.log('end checkid');
			if(err){
				console.log('checkid :'+err);
			}
			if(rows&&rows.length>0){
				if(func)
					func(true);
			}
			else{
				if(func)
					func(false);
			}
			
		});
}

module.exports = NODE;