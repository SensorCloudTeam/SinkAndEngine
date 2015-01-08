// var mysql      = require('../mysql');
var mysql = require('mysql');
var connection = mysql.createConnection({
  host     : 'localhost',
  user     : 'root',
  password : '123456'
});

var DATABASE = 'wsn';

var sqlmanager = {connection:connection,DATABASE:DATABASE};
// sqlmanager.insert = function(ph){
// 	// connection.connect();
// 	connection.query('USE '+DATABASE);
// 	connection.query(  
// 	  'INSERT INTO '+TABLE+' '+  
// 	  'SET object = ?, value = ?, provider = ?',  
// 	  [ph.oid, ph.value, ph.provider] 
// 	); 
// 	// connection.end();
// }

// sqlmanager.query = function(sqlstr){
// 	connection.query('USE '+DATABASE);
// 	connection.query(sqlstr);
// };

// module.exports = sqlmanager;

module.exports = sqlmanager;
