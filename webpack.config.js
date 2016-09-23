module.exports = function(options) {
  var path = require('path');
  var webpack = require('webpack');
  var config = {};

  config.context = path.join(__dirname, 'js');

  config.entry = {
    default: './default.js'
  };

  config.output = {
    path: path.join(__dirname, "src/js/"),
    filename: "pebble-js-app.js"
  }

  config.plugins = [
    new webpack.ProvidePlugin({
      'fetch': 'imports?this=>global!exports?global.fetch!whatwg-fetch'
    })
  ];

  config.module = {
    loaders: [
      {
        test: /\.js$/,
        exclude: /node_modules/,
        loader: "babel-loader"
      },
      {
        test: /\.json$/,
        loader: "json-loader"
      }
    ]
  }

  config.resolve = {
    modules: [path.resolve(__dirname, "js"), "node_modules"]
  };

  return config;
};
