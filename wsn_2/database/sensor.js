var sqlmanager = require('./sqlmanager.js');
var TABLE = 'sensor';
var SENSOR = {};
var connection = sqlmanager.connection;
var DATABASE = sqlmanager.DATABASE;
/*ph是这样的对象[uid:,nid:,sid:,value:,time]
*uid表示用户id,nid表示节点id,sid表示传感器id,value是传感器检测的数据值,time表示检测数据的时间*/
SENSOR.insert = function(sensor,func){
	console.log('begin insert sensor');
	connection.query('use '+DATABASE);
	connection.query( 'INSERT INTO '+TABLE+' '+ 'SET id = ?, sensor_id = ?,sink_id =?,name = ?,data_time = ?,sample_rate = ?,type_id = ?,value =? ;',  
		[sensor.id,sensor.sensor_id,sensor.sink_id,sensor.tid,sensor.name,sensor.data_time,sensor.sample_rate,sensor.type_id,sensor.value],
		function(err,rows,fields){
			console.log('end insert sensor');
			if(err)
				console.log('sensor insert :'+err);
			if(rows&&rows.length>0){
				if(func)
					func(true);
			}
			else{
				if(func)
					func(false);
			}
		});
};

SENSOR.update = function(sensor,func){
	console.log('begin update sensor');
	console.log(sensor.id);
	connection.query('use '+DATABASE);
	connection.query('UPDATE '+TABLE+' '+'SET value = ?,sample_rate = ? WHERE id = ?;',
		[sensor.value,sensor.sample_rate,sensor.id],
		function(err,rows,fields){
			console.log('end update sensor');
			if(err)
				console.log('sensor update :'+err);
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

SENSOR.insertOrUpdate = function(sensor,func){
	/*查询数据库中是否存在该传感器节点，如果有的话则执行更新数据的操作，如果没有则执行插入节点的操作*/
	SENSOR.select(sensor,function(flag){
		if(flag){
			SENSOR.update(sensor,func);
		}
		else{
			SENSOR.insert(sensor,func);
		}
	});
}

SENSOR.clear = function(){
	connection.query('use '+DATABASE);
	connection.query('truncate table '+ TABLE + ';');
}

SENSOR.select = function(sensor,func){
	console.log('begin select sensor');
	connection.query('use '+DATABASE);
	connection.query('select * from '+TABLE+ ' where id = ? ;',
		[sensor.sid,sensor.nid,sensor.uid],
		function(err,rows,fields){
			console.log('end select sensor');
			if(err)
				console.log('sensor select :'+err);
			if(rows&&rows.length>0){
				// console.log(fields);	
				if(func)
					func(true);
			}
			else{
				if(func)
					func(false);
			}
		});

}

module.exports = SENSOR;