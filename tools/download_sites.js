var fs = require('fs');
var http = require('http');

var sites_json = fs.readFileSync('top-sites.json', {
  encoding: 'utf-8'
});

var sites = JSON.parse(sites_json);
var count = 0;
var errors = [];

console.log("total sites: " + sites.length);

sites.forEach(function(site, i) {
  var request = http.get("http://" + site.site, function(res) {
    var data = '';
    res.on('data', function(chunk) {
      data += chunk;
    });
    res.on('end', function() {
      site.html = data;
      count += 1;
      console.log("ok: " + count + " " + site.site);
      fs.writeFileSync("sites.json", JSON.stringify(sites, null, 4));
    });
  });
  request.on("error", function (err) {
    errors.push(site.site);
    console.log("error: " + errors.length + " " + site.site);
    fs.writeFileSync("errors.json", JSON.stringify(errors, null, 4));
    site.html = "";
    count += 1;
    fs.writeFileSync("sites.json", JSON.stringify(sites, null, 4));
  });
});
