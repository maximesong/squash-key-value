var jsdom = require('jsdom');
var fs = require('fs');
var http = require('http');

var jquery = fs.readFileSync('./node_modules/jquery/dist/jquery.js');

var url_prefix = 'http://www.alexa.com/topsites/global;';
var extract_sites = function (html) {
  jsdom.env({
    html: html,
    src: [jquery],
    done: function(error, window) {
      var $ = window.$;
      $('li.site-listing').each(function(index, element) {
        console.log(element);
      });
    }
  });
};

// http.get('http://www.alexa.com/topsites', function(res) {
//   var data = '';
//   res.on('data', function(chunk) {
//     data += chunk;
//   });
//   res.on('end', function() {
//     fs.writeFileSync('topsites', data);
//   });
// });

var test_html = fs.readFileSync('topsites');

extract_sites(test_html);
