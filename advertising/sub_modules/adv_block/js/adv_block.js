(function ($){
   Drupal.behaviors.adv_block = {
      attach:function (context,settings){
         $(window).scroll(function(){
            showDiv();
         });   
      }
   }

function showDiv(){  
   var diffY;
   if (document.documentElement && document.documentElement.scrollTop)
      diffY = document.documentElement.scrollTop;
   else if (document.body)
      diffY = document.body.scrollTop
   document.getElementById("adv_select_id").style.top=diffY+"px";
}
})(jQuery);     