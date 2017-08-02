var express = require('express');
var mysql = require('./db.js');

var app = express();
var handlebars = require('express-handlebars').create({defaultLayout:'main'});

app.engine('handlebars', handlebars.engine);
app.set('view engine', 'handlebars');
app.set('port', 8080);

app.use(express.static('public'));

//https://github.com/wolfordj/CS290-Server-Side-Examples/blob/master/express-mysql/helloMysql.js

app.get('/',function(req,res,next){
//crate an object
  var context = {};
//get data from database
  mysql.pool.query('SELECT * FROM workout', function(err, rows, fields){
    if(err){
      next(err);
      return;
    }
  context.results = JSON.stringify(rows);
  //  res.render('home', context);
//retrieve data from database
res.send(JSON.stringify(rows));
  });
});



//https://github.com/wolfordj/CS290-Server-Side-Examples/blob/master/express-mysql/helloMysql.js
app.get('/insert',function(req,res,next){
  var context = {};
//insert data into database
  mysql.pool.query("INSERT INTO workout (`name`, `reps`,`weight`, `date`, `lbs` ) VALUES (?, ?, ?,?,?)", [req.query.name, req.query.reps, req.query.weight, req.query.date, req.query.lbs], function(err, result){
    if(err){
      next(err);
      return;
    }
    var results = "Inserted id " + result.insertId;
//retrieve data from database
	res.send(JSON.stringify(results));
  });
});


//https://github.com/wolfordj/CS290-Server-Side-Examples/blob/master/express-mysql/helloMysql.js

app.get('/delete',function(req,res,next){
  var context = {};
//delete data from database
  mysql.pool.query("DELETE FROM workout WHERE id=?", [req.query.id], function(err, result){
    if(err){
      next(err);
      return;
    }
//retrieve data from database
   var results = "Deleted " + result.changedRows + " rows.";
	res.send(JSON.stringify(results));
  });
});

//https://github.com/wolfordj/CS290-Server-Side-Examples/blob/master/express-mysql/helloMysql.js

app.get('/edit',function(req,res,next){
  var context = {};
  mysql.pool.query("SELECT * FROM workout WHERE id=?", [req.query.id], function(err, result){
    if(err){
      next(err);
      return;
    }
//if we get something that equals 1
    if(result.length == 1){
	//set the current value to the first index
      var curVals = result[0];
      mysql.pool.query("UPDATE workout SET name=?, reps=?, weight=?, date=?, lbs=? WHERE id=? ",
        [req.query.name || curVals.name, req.query.reps || curVals.reps, req.query.weight || curVals.weight, req.query.date || curVals.date, req.query.lbs || curVals.lbs, req.query.id],
        function(err, result){
        if(err){
          next(err);
          return;
        }
        var results = "Updated " + result.changedRows + " rows.";
	res.send(results);
      });
    }
  });
});

app.get('/reset-table',function(req,res,next){
  var context = {};
  mysql.pool.query("DROP TABLE IF EXISTS workout", function(err){
    var createString = "CREATE TABLE workout(" +
    "id INT PRIMARY KEY AUTO_INCREMENT," +
    "name VARCHAR(255) NOT NULL," +
    "reps INT," +
    "weight INT,"+
    "date DATE,"+
    "lbs BOOLEAN)";
    mysql.pool.query(createString, function(err){
      var results = "Table reset";
	res.send(results);
    })
  });
});

app.use(function(req,res){
  res.status(404);
  res.render('404');
});

app.use(function(err, req, res, next){
  console.error(err.stack);
  res.status(500);
  res.render('500');
});

app.listen(app.get('port'), function(){
  console.log('Express started on http://52:89:112:98:' + app.get('port') + '; press Ctrl-C to terminate.');
});
