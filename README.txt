Google analytics event tracking module README: 

Purpose
	To provide a user configurable and reusable module for the tracking of events with google analytics.
	Per this method http://code.google.com/apis/analytics/docs/tracking/eventTrackerGuide.html
	
Google method
	var _gaq = _gaq || [];
	_gaq.push(['_setAccount', 'UA-XXXXXX-YY']);
	
	(function() {
		var ga = document.createElement('script'); 
		ga.type = 'text/javascript';
		ga.async = true;
		ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';
		var s = document.getElementsByTagName('script')[0];
		s.parentNode.insertBefore(ga, s);
	})();
	
	_gaq.push(['_trackEvent', category, action, optional label, optional_value, optional noninteraction]);
	_trackEvent(category, action, opt_label, opt_value, opt_noninteraction)
	
	• category (required)
			The name you supply for the group of objects you want to track.
	• action (required)
			A string that is uniquely paired with each category, and commonly used to define the type of user interaction for the web object.
	• label (optional)
			An optional string to provide additional dimensions to the event data.
	• value (optional)
			An integer that you can use to provide numerical data about the user event.
	• non-interaction (optional)
			A boolean that when set to true, indicates that the event hit will not be used in bounce-rate calculation.