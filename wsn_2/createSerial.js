var SinkTable = require('./database/sink.js');

/*调用该函数时传入一个function参数，函数将生成的id交给该function处理*/
var createId = function(func){
	
	var id = getRandom();

	SinkTable.checkId(id,function(flag){
		if(!flag){
			func(id); 
		}
		else{
			check();
		}
	});
};

var units = ["0","1","2","3","4","5","6","7","8","9",
			"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"];
var getRandom = function(){
	var arr = [15];

	for(var k =0;k<15;k++){
		arr[k] = Math.floor(Math.random()*units.length);
	}
	var str = "";
	for(var i in arr){
		str += units[arr[i]];
	}
	return str;
};

module.exports = createId;

/*Test*/
// createId(function(id){console.log(id)});


