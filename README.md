# blueshift-react-native

React native plugin for Blueshift iOS and Android SDK.

## Installation

```sh
npm install blueshift-react-native
```

## Android and iOS Integration
Refer to the below documents to integrate the Blueshift SDK for Android and iOS.

- [Android SDK Integation](https://github.com/blueshift-labs/blueshift-react-native/blob/main/Android.md)
- [iOS SDK Integation](https://github.com/blueshift-labs/blueshift-react-native/blob/main/iOS.md)


## Usage
Import the Blueshift plugin in your JS/TS as mentioned below to use its functionality.

```js
import Blueshift from 'blueshift-react-native';
```

Once imported, you can call the Blueshift methods as mentioned below.

```js
// Make a call to Blueshift functions
Blueshift.setUserInfoEmailId("test@blueshift.com");
Blueshift.identifyWithDetails({"user_type":"premium"});

```

Refer to [these Blueshift JS methods](https://github.com/blueshift-labs/blueshift-react-native/blob/main/index.js) to know about features and methods supported by Plugin and how to use them. 

### Setting user info

Using the Plugin, you can set the logged-in user info in the Blueshift SDK. SDK will add this user data to each event and send it to the Blueshift server. This user data can be saved when the user logs in to the app and needs to be removed once the user logs out of the app.

In order to associate the events to the correct user, there are three primary identifiers used to identify a user uniquely.

- DeviceID - This will be auto-generated by SDK and sent to the Blueshift server as the `device_id` attribute as part of every event. 
- EmailId - Email id needs to be set whenever it changes as mentioned in the below code snippet and this will be sent to the Blueshift server as part of every event.
- CustomerId - Customer id needs to be set whenever it changes as mentioned in the below code snippet and this will be sent to the Blueshift server as part of every event.

```javascript
// Set data on successful login 
Blueshift.setUserInfoEmailId("test@test.com");
Blueshift.setUserInfoCustomerId("cust123456");

// Set other user info as below. This info will be used for creating 
// segments or running personalized campaigns 
Blueshift.setUserInfoFirstName("John");
Blueshift.setUserInfoLastName("Cartor");
Blueshift.setUserInfoExtras({"profession":"software engineer", "isLoggedIn":true});

// Clear the stored data at the time of logout
Blueshift.removeUserInfo();

```
Make sure you fire an `identify` event after making changes to the user data to reflect it on the Blueshift server.  

### Identify event
`Identify` event is responsible to update the data for the user profile on the Blueshift server. Whenever you change any user data in the Blueshift SDK, we recommend you fire an identify event to reflect those changes on the Blueshift server. This ensures that we can still attribute the events to the user and generate recommendations for the user if the `device_id` resets due to the app uninstall.

```javascript
// Set user data at login
Blueshift.setUserInfoEmailId("test@test.com");
Blueshift.setUserInfoCustomerId("cust123456");
Blueshift.setUserInfoExtras({"profession":"software engineer", "isLoggedIn":true});

// Identify event with custom data
Blueshift.identifyWithDetails({"user_type":"premium"});

```

### Custom Event
Use the below method to track custom events with custom data in your app.

```javascript
Blueshift.trackCustomEvent("name_of_event",{},false);

```

### Push and in-app notification opt-out
- **Push notifications opt-out**

Use Blueshift app preferences to opt-out of push notifications that we send from our platform. You can use it if you don't want push notifications from our platform to show up on your device. After modifying the value, fire an identify event to update the data on the Blueshift server.

```javascript
// set the preference for push notification
Blueshift.setEnablePush(false);

// fire identify event
Blueshift.identifyWithDetails({});

```
- **In-app notifications opt-out**

Use Blueshift app preferences to opt-out of In-app notifications that we send from our platform. You can use it if you don't want in-app notifications from our platform to show up on your device. After modifying the value, fire an identify event to update the data on the Blueshift server.

```javascript
// set the preference for push notification
Blueshift.setEnableInApp(false);

// fire identify event
Blueshift.identifyWithDetails({});

```

### Delay Push permission dialog(iOS only)
Blueshift SDK registers for iOS push notifications automatically after the app launch. If you don't want the push notification permission to be displayed immediately on the app launch, you can customize it to display it later after sign up/sign in. To do that you need to set the `config.enablePushNotification` as `false` in your **Xcode project** while initializing the Blueshift Plugin.

```Objective-c
// Disable push notifications in the SDK configuration 
// to delay the Push notification permission dialog
[config setEnablePushNotification:NO];

```

You can invoke the below plugin method from your **React Native JS/TS** when you want to register for push notifications and show the push notification dialog to the user.

```javascript
// Register for remote notifications using SDK. 
// Calling this method will show push permission dialogue to the user.
Blueshift.registerForRemoteNotification();

```
## In-App Notifications
Once you enable the in-app notifications from the SDK as mentioned in the Android and iOS set-up documents, you will need to register the screens in order to see in-app messages. You can register the screens in two ways.

- **Register all screens** Refer to [this]() Android and iOS integration documents to register all screens to receive in-app notifications. After completing this set up, in-app can be displayed on any screen when it is availble to display.

- **Register and unregister each screen** of your react native project for in-app messages. If you don’t register a screen for in-app messages, the in-app messages will stop showing up for screens that are not registered. You will need to add in-app registration and unregistration code on the `componentDidMount` and `componentWillUnmount` respectively inside your react native screens. Refer below code snipper for reference. 

```Javascript
 componentDidMount() { 
    // Register for in-app notification
    Blueshift.registerForInAppMessage("HomeScreen");
  }
  
 componentWillUnmount() {
    // Unregister for in-app notification
    Blueshift.unregisterForInAppMessage();
 }
```

## Event Listeners and Deep links

### Get Push notification payload on React Native
Blueshift Plugin will deliver the Push notification payload using the event `PushNotificationClickedEvent` when the user clicks on the push notification. Add a custom event listener using the `Blueshift` method in your react project. 

```javascript
// Add custom event listner using Blueshift method
Blueshift.addEventListener('PushNotificationClickedEvent',this.handlePushClick );

handlePushClick(payload) {
  alert("push payload "+JSON.stringify(payload));
}
```
Remove the custom event listener by calling the Blueshift `removeEventListener` method. 

```javascript
// Remove custom event listner using Blueshift method
Blueshift.removeEventListener('PushNotificationClickedEvent');
```

### Deep links for Push and In-app notifications
Blueshift Plugin will take care of delivering the deep link added inside the Push and in-app notifications to the react-native once the user interacts with the notification.

Blueshift plugin will deliver the deep link to react-native using `url` event. You will need to add an event listener using default `Linking` method as below in your react project to receive the deep link.

```javascript
componentDidMount() { 
  // Add event listner for `url` event
  global.urlListener = Linking.addEventListener('url', (event) => { 
    var url = event.url;
    if(url) {
        this.handleDeeplinkUrl(url);
      }
  });
}

componentWillUnmount() {
  // You must unregister these callbacks
  global.urlListener.remove();
}

handleDeeplinkUrl(url) {
  // Handle the received deep link to perform action.
}
```

### Deep links for Email links
Email deep links are basically App links for Android and Universal links for iOS. To enable the App links and Universal links you need to set up them as mentioned in the Android and iOS integration document.

Once set up is done, you can modify the existing `url` event listener added for Push and In-app deep links to process the email deep links. It should look like the below.

```javascript
componentDidMount() { 
  // Get the email deep link when app launched from killed state
  Linking.getInitialURL().then(url => { 
    if(url) {
      // Check if the email deep link from Blueshift
      if (Blueshift.isBlueshiftUrl(url)) {
      	  // Process the email deep link to get original url
        Blueshift.processBlueshiftUrl(url);
      }
    }
  });

  // Add event listner for `url` event
  global.urlListener = Linking.addEventListener('url', (event) => { 
    var url = event.url;
    if(url) {
      // Check if the email deep link from Blueshift
      if (Blueshift.isBlueshiftUrl(url)) {
        // Process the email deep link to get original url
        Blueshift.processBlueshiftUrl(url);
      } else {
        this.handleDeeplinkUrl(url);
      }
    }
  });
}

componentWillUnmount() {
  // You must unregister these callbacks
  global.urlListener.remove();
}

handleDeeplinkUrl(url) {
  // Handle the received deep link to perform action.
}
```

## 


## Contributing

See the [contributing guide](CONTRIBUTING.md) to learn how to contribute to the repository and the development workflow.

## License

MIT
