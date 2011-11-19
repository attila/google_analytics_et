jQuery(document).ready(function () {
  // make sure that the google analytics event tracking object exists
  // if not then exit and don't track
  if(!_gaq){
    return;
  }
  var settings = Drupal.settings.google_analytics_event_tracking_settings;
  
  var s = new Array();
  for(var i = 0; i < settings.selectors.length; i++) {
    s[i] = settings.selectors[i].selector;
  }
  
  jQuery.each(s,
    function(i, val) {
      jQuery(settings.selectors[i].selector).bind('mousedown',
        function(event) {
          trackEvent(jQuery(this), settings.selectors[i].category, settings.selectors[i].action, settings.selectors[i].label, settings.selectors[i].value, settings.selectors[i].noninteraction)
        }
      );
    }
  );
  
});

function trackEvent($obj, category, action, opt_label, opt_value, opt_noninteraction) {
  category = category == '!text' ? String($obj.text()) : String(category);
  action = action == '!text' ? String($obj.text()) : String(action);
  opt_label = opt_label == '!text' ? String($obj.text()) : String(opt_label);
  _gaq.push(['_trackEvent', String(category), String(action), String(opt_label), Number(opt_value), Boolean(opt_noninteraction)]);
}

function debugEvent($obj, category, action, opt_label, opt_value, opt_noninteraction) {
  alert(category + ' ' + action  + ' ' + opt_label + ' ' + opt_value);
}