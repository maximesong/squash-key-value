var jsdom = require('jsdom');
var fs = require('fs');

var jquery = fs.readFileSync('./node_modules/jquery/dist/jquery.js');

jsdom.env({
  html: '<div>Hello</div><a href="www.sina.com.cn">Click it</a>',
  src: [jquery],
  done: function(error, window) {
    var $ = window.$;
    $('a').each(function() {
      console.log($(this).text());
    });
  }
});
  

