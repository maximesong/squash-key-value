var jsdom = require('jsdom');
var fs = require('fs');
var http = require('http');

var jquery = fs.readFileSync('./node_modules/jquery/dist/jquery.js');

var extract_sites = function (html, callback) {
  jsdom.env({
    html: html,
    src: [jquery],
    done: function(error, window) {
      var $ = window.$;
      var sites_info = {};
      $('li.site-listing').each(function(index, element) {
        var name = $('.desc-paragraph a', element).text();
        var info_url = $('.desc-paragraph a', element).attr('href');
        sites_info[name] = {
          rank: $('.count', element).text(),
          info_url: info_url
        };
      });
      callback(sites_info);
    }
  });
};

var download_and_save = function(url, filename, callback) {
  console.log("get: " + url);
  http.get(url, function(res) {
    var data = '';
    res.on('data', function(chunk) {
      data += chunk;
    });
    res.on('end', function() {
      console.log('end');
      fs.writeFileSync(filename, data);
      if (callback) {
        callback();
      }
    });
  });
};

var download_and_save_all = function(list, callback) {
  if (list.length === 0 && callback) {
    callback();
  } else {
    var pair = list.pop();
    download_and_save(pair.url, pair.filename, function() {
      download_and_save_all(list, callback);
    });
  }
};

var mkdir = function(path) {
  if (!fs.existsSync(path)) {
    fs.mkdirSync(path);
  }
};

var download_data = function() {
  var alexa_url = 'http://www.alexa.com';
  var topsites_prefix = alexa_url + '/topsites/global;';
  var download_path = 'download';
  var sites_info_path = download_path + '/siteinfo';
  var sites_list_path = download_path + '/topsites';

  var alexa2_url = 'http://alexa.chinaz.com/?domain=';
  mkdir(download_path);
  mkdir(sites_info_path);
  mkdir(sites_list_path);

  var pair_list = [];
  (function helper(i) {
    if (i >= 20) {
      fs.writeFileSync('sites.json', JSON.stringify(pair_list, null, 4));
      return;
    }
    var html = fs.readFileSync(sites_list_path + '/sites' + i);
    extract_sites(html, function(sites) {
      for (var site in sites) {
        pair_list.push({
          site: site.toLowerCase(),
          rank: sites[site].rank
        });
      }
      helper(i + 1);
      // download_and_save_all(pair_list, function(){
      // });
    });    
  })(0);
    
};

download_data();
//var test_html = fs.readFileSync('topsites');

//extract_sites(test_html);
