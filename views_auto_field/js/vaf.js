(function ($){
 Drupal.ajax.prototype.commands.vaf_success = function(ajax, response, status)
  { 
    var row = $('.vaf-' + response.data.nid + '-manage-operations').closest('tr');
    var bg = $(row).css('backgroundColor');
    $(row).fadeOut(1000).css("background-color", "#FFFF99").fadeIn(500);
    window.setTimeout(function(){
      $(row).css('background-color', bg);
    }, 700);
  };

  Drupal.ajax.prototype.commands.vaf_error = function(ajax, response, status)
  { 
    var row = $('.vaf-' + response.data.nid + '-manage-operations').closest('tr');
    var bg = $(row).css('backgroundColor');
    $(row).fadeOut(1000).css("background-color", "red").fadeIn(500);
    $(row).find('.vaf-auditing a').text('请求失败，点击重试');
    window.setTimeout(function(){
      $(row).css('background-color', bg);
    }, 1700);
  };
})(jQuery);