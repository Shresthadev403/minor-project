$(function () {
    /* --------------------- */
  
    /*
      $.ajax({ 
      url: url, 
      dataType: "jsonp", 
      async:false, 
      jsonpCallback: "routeParseToGeojson", 
      jsonp: 'callback' 
         }); 
         
         
      $.ajax({ 
      url: url, 
      dataType: "jsonp", 
      async:false, 
      jsonpCallback: "routeParseToGeojson", 
      jsonp: 'callback' 
         }); 
         
      */
  
    // set up the map
    // Create the map
    var latitude, longitude, accuracy;
    var prev_lat, prev_lng;
  
    var map = L.map("map").setView([27.7293,85.3343], 8);
  
    // Set up the OSM layer
    L.tileLayer("http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png", {
      maxZoom: 18,
    }).addTo(map);
    // add a marker in the given location
    var marker = L.marker([27.694791, 85.320202]).addTo(map);
    var featureGroup=L.featureGroup([marker]).addTo(map);
    map.fitBounds(featureGroup.getBounds());
  
    /* --------------------- */
  
    /*  for simluation we will walk through each of the lat/lng in the array each 30 secs  */
  
    var simulation_points = [
        [27.694791, 85.320202],
        [27.694831, 85.32021],
        [27.694833, 85.320224],
        [27.694845, 85.320235],
        [27.694871, 85.320339],
        [27.694907, 85.32039],
        [27.694924, 85.320422],
        [27.69495, 85.320476],
        [27.694964, 85.320516],
        [27.694966, 85.320514],
        [27.694962, 85.320725],
        [27.694916, 85.320763],
        [27.694859, 85.320779],

        [27.694793, 85.320763],
        [27.694748, 85.320758],

        [27.694717, 85.32076],
        [27.694693, 85.32075],
        [27.69466, 85.320736],
        [27.694567, 85.320733],
        [27.694477, 85.320699],
        [27.694406, 85.320591],
        [27.69437, 85.320559],
        [27.694318, 85.320446],
        [27.694318, 85.32039],
        [27.694337, 85.320219],
        [27.694365, 85.320192],
        [27.694384, 85.320162],

           [27.694522, 85.320122],
           [27.694577, 85.320122],
           [27.694686, 85.320176],
              [27.694736, 85.320192],  
               [27.694814, 85.320202],

    ];
    var simlulated_index = $(simulation_points).size(); // set as last so on first exec will wrap to first element
  
    function simulateGeolocation() {
      /* called every 30 secs and sets the location to a different location close enough to route to */
      if (simlulated_index < $(simulation_points).size() - 1) {
        simlulated_index++;
      } else {
        simlulated_index = 0;
      }
      latitude = simulation_points[simlulated_index][0];
      longitude = simulation_points[simlulated_index][1];
      accuracy = 1;
      document.getElementById("result").innerHTML =
        "lat: " +
        latitude +
        ", " +
        "lng: " +
        longitude +
        ", " +
        "accuracy: " +
        accuracy +
        " ts: " +
        Date.now() +
        "idx: " +
        (simlulated_index + 1) +
        "  / " +
        $(simulation_points).size() +
        "<br />";
    }
  
    function setGeolocation() {
      var geolocation = window.navigator.geolocation.watchPosition(
        function (position) {
          latitude = position.coords.latitude;
          longitude = position.coords.longitude;
          accuracy = position.coords.accuracy;
  
          document.getElementById("result").innerHTML +=
            "lat: " +
            latitude +
            ", " +
            "lng: " +
            longitude +
            ", " +
            "accuracy: " +
            accuracy +
            " ts: " +
            Date.now() +
            "<br />";
        },
        function () {
          /*error*/
        },
        {
          maximumAge: 250,
          enableHighAccuracy: true,
        }
      );
    }
  
    var animation_path = []; // an array a lat/lng pairs that plot the path from start to finish of the animation for the duration before the next expected locaiton update.
  
    var animation_progress = 0; // refers to the index of the lat/lng set that
  
    function animate_marker_to_new_position(
      prev_lat,
      prev_lng,
      latitude,
      longitude
    ) {
      if (prev_lat && prev_lng) {
        // if both defined then this isn't the first
        console.log("1");
      } else {
        // otherwise this is the first call to animate
  
        console.log("no need to animate");
      }
  
      var newLatLng = new L.LatLng(latitude, longitude);
      marker.setLatLng(newLatLng);
    }
  
    var refresh_interval = 1000; // millisecs secs - 1000 = 1 sec - should be set to around 30 sec ( 30000 )
  
    // -- Run initially to set marker location
    // setGeolocation();
    simulateGeolocation();
    window.setInterval(
      function () {
        // uncomment either set or simulate .. if doing set need to ensure map starts somehwere
        // that can be routed to be OSRM ..
  
        // setGeolocation();
        simulateGeolocation();
  
        // remember the previous locations so that we can animaite from here to the next
        // .. nb could potentially extract this from the current marker location?
        //   then wouldn't need extra variables.
  
        console.log("Getting path from last known location to latest location");
        animate_marker_to_new_position(prev_lat, prev_lng, latitude, longitude);
  
        prev_lat = latitude;
        prev_lng = longitude;
  
        console.log("polling location");
  
        marker.setLatLng([latitude, longitude]).update();
      },
      refresh_interval //check every 10 seconds
    );

  
 

  
    //setGeolocation();
    //simulateGeolocation();
  });
  