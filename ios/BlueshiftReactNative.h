
#ifdef RCT_NEW_ARCH_ENABLED
#import "RNBlueshiftReactNativeSpec.h"

@interface BlueshiftReactNative : NSObject <NativeBlueshiftReactNativeSpec>
#else
#import <React/RCTBridgeModule.h>

@interface BlueshiftReactNative : NSObject <RCTBridgeModule>
#endif

@end
