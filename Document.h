
/*
     File: Document.h
 Abstract: Document object for TextEdit. 
 
  Version: 1.9
 
 Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple
 Inc. ("Apple") in consideration of your agreement to the following
 terms, and your use, installation, modification or redistribution of
 this Apple software constitutes acceptance of these terms.  If you do
 not agree with these terms, please do not use, install, modify or
 redistribute this Apple software.
 
 In consideration of your agreement to abide by the following terms, and
 subject to these terms, Apple grants you a personal, non-exclusive
 license, under Apple's copyrights in this original Apple software (the
 "Apple Software"), to use, reproduce, modify and redistribute the Apple
 Software, with or without modifications, in source and/or binary forms;
 provided that if you redistribute the Apple Software in its entirety and
 without modifications, you must retain this notice and the following
 text and disclaimers in all such redistributions of the Apple Software.
 Neither the name, trademarks, service marks or logos of Apple Inc. may
 be used to endorse or promote products derived from the Apple Software
 without specific prior written permission from Apple.  Except as
 expressly stated in this notice, no other rights or licenses, express or
 implied, are granted by Apple herein, including but not limited to any
 patent rights that may be infringed by your derivative works or by other
 works in which the Apple Software may be incorporated.
 
 The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
 MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
 THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
 OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
 
 IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
 MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
 AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
 STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 
 Copyright (C) 2013 Apple Inc. All Rights Reserved.
 
 */

#import <Cocoa/Cocoa.h>

@interface Document : NSDocument {
    // Book-keeping
    BOOL setUpPrintInfoDefaults;	/* YES the first time -printInfo is called */
    BOOL inDuplicate;
    // Document data
    NSTextStorage *textStorage;		/* The (styled) text content of the document */
    CGFloat scaleFactor;		/* The scale factor retreived from file */
    BOOL isReadOnly;			/* The document is locked and should not be modified */
    NSColor *backgroundColor;		/* The color of the document's background */
    CGFloat hyphenationFactor;		/* Hyphenation factor in range 0.0-1.0 */
    NSSize viewSize;			/* The view size, as stored in an RTF document. Can be NSZeroSize */
    BOOL hasMultiplePages;		/* Whether the document prefers a paged display */
    BOOL usesScreenFonts;		/* The document allows using screen fonts */

    // The next seven are document properties (applicable only to rich text documents)
    NSString *author;			/* Corresponds to NSAuthorDocumentAttribute */
    NSString *copyright;		/* Corresponds to NSCopyrightDocumentAttribute */
    NSString *company;			/* Corresponds to NSCompanyDocumentAttribute */
    NSString *title;			/* Corresponds to NSTitleDocumentAttribute */
    NSString *subject;			/* Corresponds to NSSubjectDocumentAttribute */
    NSString *comment;			/* Corresponds to NSCommentDocumentAttribute */
    NSArray *keywords;			/* Corresponds to NSKeywordsDocumentAttribute */
    
    // Information about how the document was created
    BOOL openedIgnoringRichText;	/* Setting at the the time the doc was open (so revert does the same thing) */
    NSStringEncoding documentEncoding;	/* NSStringEncoding used to interpret / save the document */
    BOOL convertedDocument;		/* Converted (or filtered) from some other format (and hence not writable) */
    BOOL lossyDocument;			/* Loaded lossily, so might not be a good idea to overwrite */
    BOOL transient;			/* Untitled document automatically opened and never modified */
    NSArray *originalOrientationSections; /* An array of dictionaries. Each describing the text layout orientation for a page */
    
    // Temporary information about how to save the document
    NSStringEncoding documentEncodingForSaving;	    /* NSStringEncoding for saving the document */
    NSSaveOperationType currentSaveOperation;          /* So we can know whether to use documentEncodingForSaving or documentEncoding
                                                        in -fileWrapperOfType:error: */
    
    
    // Temporary information about document's desired file type
    NSString *fileTypeToSet;		/* Actual file type determined during a read, and set after the read (which includes revert) is complete. */ 

}

- (BOOL)readFromURL:(NSURL *)absoluteURL ofType:(NSString *)typeName encoding:(NSStringEncoding)encoding ignoreRTF:(BOOL)ignoreRTF ignoreHTML:(BOOL)ignoreHTML error:(NSError **)outError;

/* Is the document rich? */
@property (nonatomic, getter=isRichText, readonly) BOOL richText;

/* Is the document read-only? */
@property (nonatomic, getter=isReadOnly) BOOL readOnly;

/* Document background color */
@property (nonatomic, copy) NSColor *backgroundColor;

/* The encoding of the document... */
@property (nonatomic) NSUInteger encoding;

/* Encoding of the document chosen when saving */
@property (nonatomic) NSUInteger encodingForSaving;

/* Whether document was converted from some other format (filter services) */
@property (nonatomic, getter=isConverted) BOOL converted;

/* Whether document was opened ignoring rich text */
@property (nonatomic, getter=isOpenedIgnoringRichText) BOOL openedIgnoringRichText;

/* Whether document was loaded lossily */
@property (nonatomic, getter=isLossy) BOOL lossy;

/* Hyphenation factor (0.0-1.0, 0.0 == disabled) */
@property (nonatomic) float hyphenationFactor;

/* View size (as it should be saved in a RTF file) */
@property (nonatomic) NSSize viewSize;

/* Scale factor; 1.0 is 100% */
@property (nonatomic) CGFloat scaleFactor;

/* Attributes */
- (NSTextStorage *)textStorage;
- (void)setTextStorage:(id)ts; // This will _copy_ the contents of the NS[Attributed]String ts into the document's textStorage.

/* Page-oriented methods */
@property (nonatomic) BOOL hasMultiplePages;
@property (nonatomic) NSSize paperSize;

/* Action methods */
- (IBAction)toggleReadOnly:(id)sender;
- (IBAction)togglePageBreaks:(id)sender;

/* Whether conversion to rich/plain be done without loss of information */
@property (nonatomic, readonly) BOOL toggleRichWillLoseInformation;

/* Default text attributes for plain or rich text formats */
- (NSDictionary *)defaultTextAttributes:(BOOL)forRichText;
- (void)applyDefaultTextAttributes:(BOOL)forRichText;

/* Document properties */
@property (nonatomic, readonly, copy) NSDictionary *documentPropertyToAttributeNameMappings;
@property (nonatomic, readonly, copy) NSArray *knownDocumentProperties;
- (void)clearDocumentProperties;
- (void)setDocumentPropertiesToDefaults;
@property (nonatomic, readonly) BOOL hasDocumentProperties;

/* Transient documents */
@property (nonatomic, getter=isTransient) BOOL transient;
@property (nonatomic, getter=isTransientAndCanBeReplaced, readonly) BOOL transientAndCanBeReplaced;

/* Layout orientation sections */
@property (nonatomic, copy) NSArray *originalOrientationSections;

/* Screen fonts property */
@property (nonatomic) BOOL usesScreenFonts;
@end
