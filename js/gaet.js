jQuery(document).ready(function () {
	// make sure that the google analytics event tracking object exists
	// if not then exit and don't track
	if(!_gaq){
		return;
	}
	var settings = Drupal.settings.google_analytics_event_tracking_settings;
	
	
	// Primary Navigation
//	$('#prinav > li > a').mousedown(
	//	function(evt){
	//		_gaq.push(['_trackEvent', 'Primary Nav', 'Click', $(this).text()]);
//		}
//	);
	
	
	
	for(var i = 0; i < settings.selectors.length; i++) {
		selector = settings.selectors[i];
		jQuery(selector.selector).mousedown(
			function(e) {
				trackEvent(jQuery(this), selector.category, selector.action, selector.label, selector.value, selector.noninteraction)
			}
		);
	}
	
});

function trackEvent($obj, category, action, opt_label, opt_value, opt_noninteraction) {
	_gaq.push(['_trackEvent', String(category), String(action), String(opt_label), Number(opt_value), Boolean(opt_noninteraction)]);
}