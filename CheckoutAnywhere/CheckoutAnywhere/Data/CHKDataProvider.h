//
//  CHKDataProvider.h
//  Checkout
//
//  Created by Shopify.
//  Copyright (c) 2015 Shopify Inc. All rights reserved.
//

@import Foundation;
#import "CHKSerializable.h"
#import <PassKit/PassKit.h>

@class CHKCart;
@class CHKCheckout;
@class CHKCreditCard;
@class CHKGiftCard;
@class CHKProduct;
@class CHKProductVariant;
@class CHKShop;

typedef NS_ENUM(NSUInteger, CHKStatus) {
	CHKStatusComplete = 200,
	CHKStatusProcessing = 202,
	CHKStatusNotFound = 404,
	CHKStatusPreconditionFailed = 412,
	CHKStatusFailed = 424,
	CHKStatusUnknown
};

typedef void (^CHKDataCreditCardBlock)(CHKCheckout *checkout, NSString *paymentSessionId, NSError *error);
typedef void (^CHKDataCheckoutBlock)(CHKCheckout *checkout, NSError *error);
typedef void (^CHKDataCheckoutStatusBlock)(CHKCheckout *checkout, CHKStatus status, NSError *error);
typedef void (^CHKDataShippingRatesBlock)(NSArray *shippingRates, CHKStatus status, NSError *error);
typedef void (^CHKDataShopBlock)(CHKShop *shop, NSError *error);
typedef void (^CHKDataProductBlock)(CHKProduct *product, NSError *error);
typedef void (^CHKDataProductListBlock)(NSArray *products, NSUInteger page, BOOL reachedEnd, NSError *error);
typedef void (^CHKDataImagesListBlock)(NSArray *images, NSError *error);
typedef void (^CHKDataGiftCardBlock)(CHKGiftCard *giftCard, NSError *error);

@interface CHKDataProvider : NSObject

- (instancetype)initWithShopDomain:(NSString *)shopDomain apiKey:(NSString *)apiKey channelId:(NSString *)channelId;

/**
 *  The page size for any paged request. This can range from 1-250.
 */
@property (nonatomic, assign) NSUInteger pageSize;

/**
 *  The shop domain set using the initializer
 */
@property (nonatomic, strong, readonly) NSString *shopDomain;

/**
 *  The API Key set using the initializer
 */
@property (nonatomic, strong, readonly) NSString *apiKey;

/**
 *  The Channel ID set using the initializer
 */
@property (nonatomic, strong, readonly) NSString *channelId;

/**
 *  Application name to attribute orders to.  Defaults to app bundle name (CFBundleName)
 */
@property (nonatomic, strong) NSString *applicationName;

#pragma mark - Storefront

/**
 *  Fetches the shop's metadata (from /meta.json).
 *
 *  @param block (^CHKDataShopBlock)(CHKShop *shop, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)getShop:(CHKDataShopBlock)block;

/**
 *  Fetches a single page of products for the shop.
 *
 *  @param page  Page to request. Pages start at 1.
 *  @param block (^CHKDataProductListBlock)(NSArray *products, NSUInteger page, BOOL reachedEnd, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)getProductsPage:(NSUInteger)page completion:(CHKDataProductListBlock)block;

/**
 *  Fetches a single product by the name of the product. 
 *  For example, if the product url is: http://_________.myshopify.com/products/BANANA
 *
 *  @param handle Product handle
 *  @param block  (^CHKDataProductBlock)(CHKProduct *product, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)getProductByHandle:(NSString *)handle completion:(CHKDataProductBlock)block;

#pragma mark - Checkout

/**
 *  Builds a checkout on Shopify. The checkout object is used to prepare an order
 *
 *  @param checkout CHKCheckout to create on Shopify
 *  @param block    (^CHKDataCheckoutBlock)(CHKCheckout *checkout, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)createCheckout:(CHKCheckout *)checkout completion:(CHKDataCheckoutBlock)block;

/**
 *  Builds a checkout on Shopify using a Cart Token from an existing cart on your Shopify store's storefront. 
 *  The CHKCheckout object is used to prepare an order.
 *
 *  @param cartToken Cart Token associated with an existing CHKCheckout on Shopify
 *  @param block     (^CHKDataCheckoutBlock)(CHKCheckout *checkout, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)createCheckoutWithCartToken:(NSString *)cartToken completion:(CHKDataCheckoutBlock)block;

/**
 *  Applies a gift card code to the checkout.
 *
 *  @param giftCardCode The gift card code to apply on an existing checkout on Shopify. Note: This is not the same as the gift card identifier.
 *  @param checkout     An existing CHKCheckout on Shopify
 *  @param block        (^CHKDataGiftCardBlock)(CHKGiftCard *giftCard, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)applyGiftCardWithCode:(NSString *)giftCardCode toCheckout:(CHKCheckout *)checkout completion:(CHKDataGiftCardBlock)block;

/**
 *  Removes a gift card from the checkout.
 *
 *  @param giftCard The CHKGiftCard to remove
 *  @param checkout The CHKCheckout for which the Gift Card should be removed
 *  @param block    (^CHKDataGiftCardBlock)(CHKGiftCard *giftCard, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)removeGiftCard:(CHKGiftCard *)giftCard fromCheckout:(CHKCheckout *)checkout completion:(CHKDataGiftCardBlock)block;

/**
 *  Retrieves an updated version of a CHKCheckout from Shopify.
 *
 *  Note: There's no guarantee that the CHKCheckout returned will be the same as the one that is passed in.
 *  We recommended using the CHKCheckout returned in the block.
 *
 *  @param checkout The CHKCheckout to retrieve (updated) from Shopify
 *  @param block    (^CHKDataCheckoutBlock)(CHKCheckout *checkout, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)getCheckout:(CHKCheckout *)checkout completion:(CHKDataCheckoutBlock)block;

/**
 *  Updates a given CHKCheckout on Shopify.
 *
 *  Note: There's no guarantee that the CHKCheckout returned will be the same as the one that is passed in.
 *  We recommended using the CHKCheckout returned in the block.
 *
 *  @param checkout The CHKCheckout to updated on Shopify
 *  @param block    (^CHKDataCheckoutBlock)(CHKCheckout *checkout, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)updateCheckout:(CHKCheckout *)checkout completion:(CHKDataCheckoutBlock)block;

/**
 *  Finalizes the CHKCheckout and charges the credit card. 
 *  This enqueues a completion job on Shopify and returns immediately.
 *  You must get the job's status by calling checkCompletionStatusOfCheckout:block
 *
 *  Note: There's no guarantee that the CHKCheckout returned will be the same as the one that is passed in.
 *  We recommended using the CHKCheckout returned in the block.
 *
 *  @param checkout The CHKCheckout to complete
 *  @param block    (^CHKDataCheckoutBlock)(CHKCheckout *checkout, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)completeCheckout:(CHKCheckout *)checkout completion:(CHKDataCheckoutBlock)block;

/**
 */
/**
 *  Finalizes the checkout and charges the credit card associated with the payment token from PassKit (Apple Pay). 
 *  This only enqueues a completion job, and will return immediately.
 *  You must get the job's status by calling checkCompletionStatusOfCheckout:block
 *
 *  Note: There's no guarantee that the CHKCheckout returned will be the same as the one that is passed in.
 *  We recommended using the CHKCheckout returned in the block.
 *
 *  @param checkout The CHKCheckout to complete
 *  @param token    The PKPaymentToken
 *  @param block    (^CHKDataCheckoutBlock)(CHKCheckout *checkout, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)completeCheckout:(CHKCheckout *)checkout withApplePayToken:(PKPaymentToken *)token completion:(CHKDataCheckoutBlock)block;

/**
 *  Retrieve the status of a CHKCheckout. This checks the status of the current payment processing job for the provided checkout.
 *  Once the job is complete (status == CHKStatusComplete), you can use the `orderId` property on CHKCheckout to retrieve the associated order.
 *
 *  Note: There's no guarantee that the CHKCheckout returned will be the same as the one that is passed in.
 *  We recommended using the CHKCheckout returned in the block.
 *
 *  @param checkout The CHKCheckout to retrieve completion status for
 *  @param block    (^CHKDataCheckoutStatusBlock)(CHKCheckout *checkout, CHKStatus status, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)getCompletionStatusOfCheckout:(CHKCheckout *)checkout completion:(CHKDataCheckoutStatusBlock)block;

#pragma mark - Shipping Rates

/**
 *  Retrieves a list of applicable shipping rates for a given CHKCheckout. 
 *  Add the preferred/selected CHKShippingRate to CHKCheckout, then update CHKCheckout
 *
 *  @param checkout The CHKCheckout to retrieve shipping rates for
 *  @param block    (^CHKDataShippingRatesBlock)(NSArray *shippingRates, CHKStatus status, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)getShippingRatesForCheckout:(CHKCheckout *)checkout completion:(CHKDataShippingRatesBlock)block;

#pragma mark - Payment Management

/**
 *  Prepares a credit card for usage during the checkout process. This sends it to Shopify's secure servers.
 *  Note: Storing the token does not charge the associated card (credit or otherwise).
 *  The card will be charged upon finalizing the checkout (completeCheckout:completion:)
 *
 *  @param creditCard CHKCreditCard to prepare for usage
 *  @param checkout   The CHKCheckout associated to the purchase.
 *                    The `billingAddress` stored on the CHKCheckout object is optional and recommended and
 *                    used (if provided) to help with fraud checking.
 *  @param block      (^CHKDataCreditCardBlock)(CHKCheckout *checkout, NSString *paymentSessionId, NSError *error);
 *
 *  @return The associated NSURLSessionDataTask
 */
- (NSURLSessionDataTask *)storeCreditCard:(id <CHKSerializable>)creditCard checkout:(CHKCheckout *)checkout completion:(CHKDataCreditCardBlock)block;

@end
