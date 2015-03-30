/*日期格式化
*例子：var nowStr = now.format("yyyy-MM-dd hh:mm:ss"); */
Date.prototype.format = function(format){ 
	var o = { 
		"M+" : this.getMonth()+1, //month 
		"d+" : this.getDate(), //day 
		"h+" : this.getHours(), //hour 
		"m+" : this.getMinutes(), //minute 
		"s+" : this.getSeconds(), //second 
		"q+" : Math.floor((this.getMonth()+3)/3), //quarter 
		"S" : this.getMilliseconds() //millisecond 
	}; 

	if(/(y+)/.test(format)) { 
		format = format.replace(RegExp.$1, (this.getFullYear()+"").substr(4 - RegExp.$1.length)); 
	} 

	for(var k in o) { 
		if(new RegExp("("+ k +")").test(format)) { 
			format = format.replace(RegExp.$1, RegExp.$1.length==1 ? o[k] : ("00"+ o[k]).substr((""+ o[k]).length)); 
		} 
	} 
	return format; 
};
/*去除字符串首尾空格*/
String.prototype.Trim = function()  
{  
	return this.replace(/(^\s*)|(\s*$)/g, "");  
};  
String.prototype.endWith=function(endStr){
  var d=this.length-endStr.length;
  return (d>=0&&this.lastIndexOf(endStr)==d);
};
var net = require('net');
// var xmlparser = require('xml-parser');
// var xmlbuilder = require('xmlbuilder');

var SinkTable = require('./database/sink.js');
var SensorTable = require('./database/sensor.js');

var ip = require('./configure.js');
var HOST = ip.host;
var PORT = ip.port;

/*注册一个socket端口*/
var server = net.createServer();
server.listen(PORT,HOST);
// console.log(server);
console.log('--------------Server Start----------------');
console.log('Server listening on '+HOST +':'+PORT);

var count = 0;
// var message = '';

server.on('connection',function(socket){
	console.log('CONNECTED: '+socket.remoteAddress+':'+socket.remotePort);

	// 为这个socket实例添加一个"data"事件处理函数  
	socket.on('data', function(data) { 
		// console.log('DATA'+": "+data); 
		// socket.write('success!');

		var pattern_request= new RegExp("^{\"Request\"");
		//如果是請求數據
		if(pattern_request.test(data)){
			//处理请求数据
			console.log("Request");
			var command;
			//如果sinkid存在
			parserRequest(data,function(flag){
				if(flag){
					command = "ok";
				}
				else{
					command = "failed";
				}
				var response = composeResponse(command); //编辑应答数据
				socket.write(response);
			});
			
			
		}
		else{
			/*data中存放的是一段长度的数据，当从一个data中截取出原始数据段可能会造成原始数据段的割裂。
			*比如原始数据段的长度为10，data的长度为52，那么在第一个data中包含前5个完整的原始数据段和一个长度为2的不完整的原始数据段；
			*解决方法：检查通过截取所获得的原始数据段的某位是否符合指定的格式*/
			var lost = "";
			var ss = data.toString().split("{\"Data");
			for(var i in ss){
				if(lost !== ""){
					ss[i] = lost+ss[i];
					lost = "";
				}
				console.log(i+":"+ss[i].toString());
				if(ss[i].endWith("}}")){
					// console.log("开始处理:"+ss[i]);
					(function(i){
						
						parserData("{\"Data"+ss[i]);
						
					})(i);
				}else{
					lost = ss[i];
				}
			}
		}
		

    });  

    // 为这个socket实例添加一个"close"事件处理函数  
    socket.on('close', function(data) {  
    	console.log('CLOSED: '+data); 
    });  
});

function parserData(data){
	try{
		var data_json = JSON.parse(data.toString());

		// console.log(JSON.stringify(data_json));

		var sink_id = data_json.Data.sinkid;
		console.log("sink:"+sink_id);
		var values = data_json.Data.values;
		// var datas = [];
		for(var i =0;i<values.length;i++){
			var sensor = {};
			sensor.sensor_id = values[i].sensorid;
			sensor.sink_id = sink_id;
			sensor.value = values[i].value;
			sensor.id = sensor.sink_id+"_"+sensor.sensor_id;
			var date = new Date();
			sensor.data_time = date;
			SensorTable.update(sensor);
		}
	}catch(e){
		console.log("error:"+e.message);
	}
}

function parserRequest(data,func){
	var request = JSON.parse(data.toString());

	console.log(JSON.stringify(request));

	var sinkid = request.Request.sinkid;
	SinkTable.checkId(sinkid,func);
}

function composeResponse(f){

	var response = {"Answer":{"command":f}};
	console.log(JSON.stringify(response));
	return JSON.stringify(response);
}

// /*解析xml*/
// function parser(xml){
// 	console.log('解析：'+xml)
// 	xml = xml+'';
// 	var obj = xmlparser(xml);

// 	try{
// 		if(obj&&obj.root){

// 			// /*接受注册数据*/
// 			// if(obj.root.name == 'GateProtocol'){
// 			// 	console.log('------------注册信息--------------');
// 			// 	var sensors = obj.root.children[5].children[0].children;
// 			// 	var node = obj.root.children[5].children[1].children[0];
// 			// 	var nodedata = {uid:1,nid:node.attributes.id,name:node.content};

// 			// 	/*插入传感器信息的函数*/
// 			// 	var insertSensor = function(){
// 			// 		console.log('test -- ss:'+sensors.length);
// 			// 		for(var k in sensors){
// 			// 			(function(k){
// 			// 				var sdata = {
// 			// 					sid:sensors[k].attributes.id,
// 			// 					nid:nodedata.nid,
// 			// 					uid:nodedata.uid,
// 			// 					tid:2,
// 			// 					name:sensors[k].content
// 			// 				};
// 			// 				SensorTable.select(sdata,function(){
// 			// 					// console.log('k:'+k);
// 			// 					SensorTable.insert(sdata);
// 			// 				});
// 			// 			})(k);

// 			// 		}
// 			// 	};
// 			// 	/*插入数据库*/
// 			// 	SinkTable.select(nodedata,function(flag){
// 			// 		//flag表示指定的node不在数据库中
// 			// 		if(!flag){
// 			// 			SinkTable.insert(nodedata,insertSensor); 
// 			// 		}
// 			// 		else{
// 			// 			insertSensor();
// 			// 		}
					
// 			// 	});

// 			// 	/*编辑回应node的xml格式的信息*/
// 			// 	var date = new Date();
// 			// 	date = date.format('yyyyMMddhhmmss');
// 			// 	/*发送应答*/
// 			// 	var response = {GateProtocol:{
// 			// 		'@version':'1.0',
// 			// 		cmd:{
// 			// 			'#text':'GateRegisterResponse'
// 			// 		},
// 			// 		timestamp:{
// 			// 			'#text':date
// 			// 		},
// 			// 		success:{
// 			// 			'#text':'true'
// 			// 		}
// 			// 	}};
// 			// 	var rxml = xmlbuilder.create(response);
// 			// 	return rxml;

// 			// }
			
// 			/*接受检测数据*/
// 			if(obj.root.name == 'DataFrame'){
// 				console.log('------------接受检测数据--------------');
// 				var timestamp = obj.root.children[0];
// 				var values = obj.root.children[1].children;

// 				for(var i in values){
// 					var ph = {};
// 					ph.value = values[i].content;
// 					ph.nid = values[i].attributes.oid;
// 					ph.time = timestamp.content;
// 					ph.sid = values[i].attributes.mid;
// 					ph.uid = 1;

// 					PHTable.insert(ph); 
// 				}
// 			}
// 		}
// 	}catch(e){
// 		console.log("err:"+obj.root.children);
// 	}
// }; 



