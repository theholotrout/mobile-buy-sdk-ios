//
//  BUYProductViewFooter.m
//  Mobile Buy SDK
//
//  Created by Rune Madsen on 2015-07-07.
//  Copyright (c) 2015 Shopify Inc. All rights reserved.
//

#import "BUYProductViewFooter.h"

@interface BUYProductViewFooter ()

@property (nonatomic, strong) UIVisualEffectView *visualEffectView;
@property (nonatomic, strong) NSArray *checkoutLayoutConstraints;
@property (nonatomic, strong) NSArray *applePayLayoutConstraints;

@end

@implementation BUYProductViewFooter

- (instancetype)init
{
	self = [super init];
	if (self) {
		self.backgroundColor = [UIColor clearColor];
		
		UIBlurEffect *blurEffect = [UIBlurEffect effectWithStyle:UIBlurEffectStyleLight];
		self.visualEffectView = [[UIVisualEffectView alloc] initWithEffect:blurEffect];
		self.visualEffectView.translatesAutoresizingMaskIntoConstraints = NO;
		[self addSubview:self.visualEffectView];
		
		[self addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[_visualEffectView]|"
																	 options:0
																	 metrics:nil
																	   views:NSDictionaryOfVariableBindings(_visualEffectView)]];
		[self addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|[_visualEffectView]|"
																	 options:0
																	 metrics:nil
																	   views:NSDictionaryOfVariableBindings(_visualEffectView)]];
		
		UIView *separatorLineView = [[UIView alloc] init];
		separatorLineView.translatesAutoresizingMaskIntoConstraints = NO;
		separatorLineView.backgroundColor = [UIColor colorWithWhite:0 alpha:0.25];
		[self.visualEffectView.contentView addSubview:separatorLineView];
		
		[self.visualEffectView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[separatorLineView]|"
																					  options:0
																					  metrics:nil
																						views:NSDictionaryOfVariableBindings(separatorLineView)]];
		[self.visualEffectView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|[separatorLineView(lineHeight)]"
																					  options:0
																					  metrics:@{ @"lineHeight" : @([[UIScreen mainScreen] scale] / 4) }
																						views:NSDictionaryOfVariableBindings(separatorLineView)]];
		
		self.checkoutButton = [UIButton buttonWithType:UIButtonTypeSystem];
		self.checkoutButton.translatesAutoresizingMaskIntoConstraints = NO;
		[self.checkoutButton setTitle:@"Checkout" forState:UIControlStateNormal];
		[self.checkoutButton setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
		self.checkoutButton.backgroundColor = self.tintColor;
		self.checkoutButton.layer.cornerRadius = 5;
		[self.visualEffectView.contentView addSubview:self.checkoutButton];
		
		self.buyPaymentButton = [BUYPaymentButton buttonWithType:BUYPaymentButtonTypeBuy style:BUYPaymentButtonStyleBlack];
		self.buyPaymentButton.translatesAutoresizingMaskIntoConstraints = NO;
		[self.visualEffectView.contentView addSubview:self.buyPaymentButton];
		
		NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(_checkoutButton, _buyPaymentButton);
		
		self.applePayLayoutConstraints = [NSLayoutConstraint constraintsWithVisualFormat:@"H:|-[_checkoutButton]-[_buyPaymentButton(==_checkoutButton)]-|"
																				 options:0
																				 metrics:nil
																				   views:viewsDictionary];
		[self addConstraints:self.applePayLayoutConstraints];
		
		self.checkoutLayoutConstraints = [NSLayoutConstraint constraintsWithVisualFormat:@"H:|-[_checkoutButton]-|"
																				options:0
																				metrics:nil
																				  views:viewsDictionary];
		[self addConstraints:self.checkoutLayoutConstraints];
		
		NSDictionary *metricsDictionary = @{ @"buttonHeight" : @(44.0f) };
		
		[self.visualEffectView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-[_checkoutButton(buttonHeight)]-|"
																					  options:0
																					  metrics:metricsDictionary
																						views:viewsDictionary]];
		[self.visualEffectView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-[_buyPaymentButton(==_checkoutButton)]-|"
																					  options:0
																					  metrics:nil
																						views:viewsDictionary]];
	}
	return self;
}

- (void)setApplePayButtonVisible:(BOOL)isApplePayAvailable
{
	self.buyPaymentButton.hidden = !isApplePayAvailable;
	if (isApplePayAvailable) {
		[NSLayoutConstraint activateConstraints:self.applePayLayoutConstraints];
		[NSLayoutConstraint deactivateConstraints:self.checkoutLayoutConstraints];
	} else {
		[NSLayoutConstraint activateConstraints:self.checkoutLayoutConstraints];
		[NSLayoutConstraint deactivateConstraints:self.applePayLayoutConstraints];
	}
}

- (void)tintColorDidChange
{
	[super tintColorDidChange];
	self.checkoutButton.backgroundColor = self.tintColor;
}

/*
 // Only override drawRect: if you perform custom drawing.
 // An empty implementation adversely affects performance during animation.
 - (void)drawRect:(CGRect)rect {
 // Drawing code
 }
 */

@end