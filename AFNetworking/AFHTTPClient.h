// AFHTTPClient.h
//
// Copyright (c) 2011 Gowalla (http://gowalla.com/)
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#import <Foundation/Foundation.h>
#import "AFHTTPRequestOperation.h"

@protocol AFMultipartFormData;

/**
 `AFHTTPClient` objects encapsulates the common patterns of communicating with an application, webservice, or API. It encapsulates persistent information, like base URL, authorization credentials, and HTTP headers, and uses them to construct and manage the execution of HTTP request operations.
 
 In its default implementation, `AFHTTPClient` sets the following HTTP headers:
 
 - `Accept: application/json`
 - `Accept-Encoding: gzip`
 - `Accept-Language: #{[NSLocale preferredLanguages]}, en-us;q=0.8`
 - `User-Agent: #{generated user agent}`
 
 You can override these HTTP headers or define new ones using `setDefaultHeader:value:`. 
 
 # Subclassing Notes
 
 It is strongly recommended that you create an `AFHTTPClient` subclass for each website or web application that your application communicates with, and in each subclass, defining a method that returns a singleton object that acts as single a shared HTTP client that holds authentication credentials and other configuration for the entire application.
 
 ## Methods to Override
 
 If an `AFHTTPClient` wishes to change the way request parameters are encoded, then the base implementation of `requestWithMethod:path:parameters:` should be overridden. Otherwise, it should be sufficient to take the `super` implementation, and configure the resulting `NSMutableURLRequest` object accordingly.
 */
@interface AFHTTPClient : NSObject {
@private
    NSURL *_baseURL;
    NSStringEncoding _stringEncoding;
    NSMutableDictionary *_defaultHeaders;
    NSOperationQueue *_operationQueue;
}

///---------------------------------------
/// @name Accessing HTTP Client Properties
///---------------------------------------

/**
 The url used as the base for paths specified in methods such as `getPath:parameteres:success:failure`
 */
@property (readonly, nonatomic, retain) NSURL *baseURL;

/**
 The string encoding used in constructing url requests. This is `NSUTF8StringEncoding` by default.
 */
@property (nonatomic, assign) NSStringEncoding stringEncoding;

/**
 The operation queue which manages operations enqueued by the HTTP client.
 */
@property (readonly, nonatomic, retain) NSOperationQueue *operationQueue;

///---------------------------------------------
/// @name Creating and Initializing HTTP Clients
///---------------------------------------------

/**
 Creates and initializes an `AFHTTPClient` object with the specified base URL.
 
 @param url The base URL for the HTTP client. This argument must not be nil.
  
 @return The newly-initialized HTTP client
 */
+ (AFHTTPClient *)clientWithBaseURL:(NSURL *)url;

/**
 Initializes an `AFHTTPClient` object with the specified base URL.
 
 @param url The base URL for the HTTP client. This argument must not be nil.
 
 @discussion This is the designated initializer for `AFHTTPClient`
 
 @return The newly-initialized HTTP client
 */
- (id)initWithBaseURL:(NSURL *)url;

///----------------------------------
/// @name Managing HTTP Header Values
///----------------------------------

/**
 Returns the value for the HTTP headers set in request objects created by the HTTP client.
 
 @param header The HTTP header to return the default value for
 
 @return The default value for the HTTP header, or `nil` if unspecified
 */
- (NSString *)defaultValueForHeader:(NSString *)header;

/**
 Sets the value for the HTTP headers set in request objects made by the HTTP client. If `nil`, removes the existing value for that header.
 
 @param header The HTTP header to set a default value for
 @param value The value set as default for the specified header, or `nil
 */
- (void)setDefaultHeader:(NSString *)header value:(NSString *)value;

/**
 Sets the "Authorization" HTTP header set in request objects made by the HTTP client to a basic authentication value with Base64-encoded username and password. This overwrites any existing value for this header.
 
 @param username The HTTP basic auth username
 @param password The HTTP basic auth password
 */
- (void)setAuthorizationHeaderWithUsername:(NSString *)username password:(NSString *)password;

/**
 Sets the "Authorization" HTTP header set in request objects made by the HTTP client to a token-based authentication value, such as an OAuth access token. This overwrites any existing value for this header.
 
 @param token The authentication token
 */
- (void)setAuthorizationHeaderWithToken:(NSString *)token;

/**
 Clears any existing value for the "Authorization" HTTP header.
 */
- (void)clearAuthorizationHeader;

///-------------------------------
/// @name Creating Request Objects
///-------------------------------

/**
 Creates an `NSMutableURLRequest` object with the specified HTTP method and path. If the HTTP method is `GET`, the parameters will be used to construct a url-encoded query string that is appended to the request's URL. If `POST`, `PUT`, or `DELETE`, the parameters will be encoded into a `application/x-www-form-urlencoded` HTTP body.
 
 @param method The HTTP method for the request, such as `GET`, `POST`, `PUT`, or `DELETE`.
 @param path The path to be appended to the HTTP client's base URL and used as the request URL.
 @param parameters The parameters to be either set as a query string for `GET` requests, or the request HTTP body.
 
 @return An `NSMutableURLRequest` object
 */
- (NSMutableURLRequest *)requestWithMethod:(NSString *)method 
                                      path:(NSString *)path 
                                parameters:(NSDictionary *)parameters;

/**
 Creates an `NSMutableURLRequest` object with the specified HTTP method and path, and constructs a `multipart/form-data` HTTP body, using the specified parameters and multipart form data block. See http://www.w3.org/TR/html4/interact/forms.html#h-17.13.4.2
 
 @param method The HTTP method for the request. Must be either `POST`, `PUT`, or `DELETE`.
 @param path The path to be appended to the HTTP client's base URL and used as the request URL.
 @param parameters The parameters to be encoded and set in the request HTTP body.
 @param block A block that takes a single argument and appends data to the HTTP body. The block argument is an object adopting the `AFMultipartFormData` protocol. This can be used to upload files, encode HTTP body as JSON or XML, or specify multiple values for the same parameter, as one might for array values.
 
 @see AFMultipartFormData
 
 @return An `NSMutableURLRequest` object
 */
- (NSMutableURLRequest *)multipartFormRequestWithMethod:(NSString *)method
                                                   path:(NSString *)path
                                             parameters:(NSDictionary *)parameters
                              constructingBodyWithBlock:(void (^)(id <AFMultipartFormData> formData))block;


///--------------------------------
/// @name Enqueuing HTTP Operations
///--------------------------------

/**
 Creates and enqueues an `AFHTTPRequestOperation` to the HTTP client's operation queue.
 
 @param request The request object to be loaded asynchronously during execution of the operation.
 @param success A block object to be executed when the request operation finishes successfully, with a status code in the 2xx range, and with an acceptable content type (e.g. `application/json`). This block has no return value and takes a single argument, which is an object created from the response data of request.
 @param failure A block object to be executed when the request operation finishes unsuccessfully, or that finishes successfully, but encountered an error while parsing the resonse data as JSON. This block has no return value and takes a single argument, which is the `NSError` object describing the network or parsing error that occurred.
 */
- (void)enqueueHTTPOperationWithRequest:(NSURLRequest *)request 
                                success:(void (^)(id object))success 
                                failure:(void (^)(NSHTTPURLResponse *response, NSError *error))failure;

///---------------------------------
/// @name Cancelling HTTP Operations
///---------------------------------

/**
 Cancels all operations in the HTTP client's operation queue that match the specified HTTP method and URL.
 
 @param method The HTTP method to match for the cancelled requests, such as `GET`, `POST`, `PUT`, or `DELETE`.
 @param url The URL to match for the cancelled requests.
 */
- (void)cancelHTTPOperationsWithMethod:(NSString *)method andURL:(NSURL *)url;

///---------------------------
/// @name Making HTTP Requests
///---------------------------

/**
 Creates an `AFHTTPRequestOperation` with a `GET` request, and enqueues it to the HTTP client's operation queue.
 
 @param path The path to be appended to the HTTP client's base URL and used as the request URL.
 @param parameters The parameters to be encoded and appended as the query string for the request URL.
 @param success A block object to be executed when the request operation finishes successfully, with a status code in the 2xx range, and with an acceptable content type (e.g. `application/json`). This block has no return value and takes a single argument, which is the response object created from the response data of request.
 @param failure A block object to be executed when the request operation finishes unsuccessfully, or that finishes successfully, but encountered an error while parsing the resonse data as JSON. This block has no return value and takes a single argument, which is the `NSError` object describing the network or parsing error that occurred.
 
 @see enqueueHTTPOperationWithRequest:success:failure
 */
- (void)getPath:(NSString *)path
     parameters:(NSDictionary *)parameters
        success:(void (^)(id object))success 
        failure:(void (^)(NSHTTPURLResponse *response, NSError *error))failure;

/**
 Creates an `AFHTTPRequestOperation` with a `POST` request, and enqueues it to the HTTP client's operation queue.
 
 @param path The path to be appended to the HTTP client's base URL and used as the request URL.
 @param parameters The parameters to be encoded and set in the request HTTP body.
 @param success A block object to be executed when the request operation finishes successfully, with a status code in the 2xx range, and with an acceptable content type (e.g. `application/json`). This block has no return value and takes a single argument, which is the response object created from the response data of request.
 @param failure A block object to be executed when the request operation finishes unsuccessfully, or that finishes successfully, but encountered an error while parsing the resonse data as JSON. This block has no return value and takes a single argument, which is the `NSError` object describing the network or parsing error that occurred.
 
 @see enqueueHTTPOperationWithRequest:success:failure
 */
- (void)postPath:(NSString *)path 
      parameters:(NSDictionary *)parameters 
         success:(void (^)(id object))success 
         failure:(void (^)(NSHTTPURLResponse *response, NSError *error))failure;

/**
 Creates an `AFHTTPRequestOperation` with a `PUT` request, and enqueues it to the HTTP client's operation queue.
 
 @param path The path to be appended to the HTTP client's base URL and used as the request URL.
 @param parameters The parameters to be encoded and set in the request HTTP body.
 @param success A block object to be executed when the request operation finishes successfully, with a status code in the 2xx range, and with an acceptable content type (e.g. `application/json`). This block has no return value and takes a single argument, which is the response object created from the response data of request.
 @param failure A block object to be executed when the request operation finishes unsuccessfully, or that finishes successfully, but encountered an error while parsing the resonse data as JSON. This block has no return value and takes a single argument, which is the `NSError` object describing the network or parsing error that occurred.
 
 @see enqueueHTTPOperationWithRequest:success:failure
 */
- (void)putPath:(NSString *)path 
     parameters:(NSDictionary *)parameters 
        success:(void (^)(id object))success 
        failure:(void (^)(NSHTTPURLResponse *response, NSError *error))failure;

/**
 Creates an `AFHTTPRequestOperation` with a `DELETE` request, and enqueues it to the HTTP client's operation queue.
 
 @param path The path to be appended to the HTTP client's base URL and used as the request URL.
 @param parameters The parameters to be encoded and set in the request HTTP body.
 @param success A block object to be executed when the request operation finishes successfully, with a status code in the 2xx range, and with an acceptable content type (e.g. `application/json`). This block has no return value and takes a single argument, which is the response object created from the response data of request.
 @param failure A block object to be executed when the request operation finishes unsuccessfully, or that finishes successfully, but encountered an error while parsing the resonse data as JSON. This block has no return value and takes a single argument, which is the `NSError` object describing the network or parsing error that occurred.
 
 @see enqueueHTTPOperationWithRequest:success:failure
 */
- (void)deletePath:(NSString *)path 
        parameters:(NSDictionary *)parameters 
           success:(void (^)(id object))success 
           failure:(void (^)(NSHTTPURLResponse *response, NSError *error))failure;
@end

#pragma mark -

/**
 The `AFMultipartFormData` protocol defines the methods supported by the parameter in the block argument of `multipartFormRequestWithMethod:path:parameters:constructingBodyWithBlock:`.
 */
@protocol AFMultipartFormData

/**
 Appends HTTP headers, followed by the encoded data and the multipart form boundary.
 
 @param headers The HTTP headers to be appended to the form data.
 @param body The data to be encoded and appended to the form data.
 */
- (void)appendPartWithHeaders:(NSDictionary *)headers body:(NSData *)body;

/**
 Appends the HTTP headers `Content-Disposition: form-data; name=#{name}"`, followed by the encoded data and the multipart form boundary.
 
 @param data The data to be encoded and appended to the form data.
 @param name The name to be associated with the specified data. This parameter must not be `nil`.
 */
- (void)appendPartWithFormData:(NSData *)data name:(NSString *)name;

/**
 Appends the HTTP header `Content-Disposition: file; filename=#{generated filename}; name=#{name}"` and `Content-Type: #{mimeType}`, followed by the encoded file data and the multipart form boundary.
 
 @param data The data to be encoded and appended to the form data.
 @param mimeType The MIME type of the specified data. (For example, the MIME type for a JPEG image is image/jpeg.) For a list of valid MIME types, see http://www.iana.org/assignments/media-types/. This parameter must not be `nil`.
 @param name The name to be associated with the specified data. This parameter must not be `nil`.
 
 @discussion The filename associated with this data in the form will be automatically generated using the parameter name specified and a unique timestamp-based hash.  
 */
- (void)appendPartWithFileData:(NSData *)data mimeType:(NSString *)mimeType name:(NSString *)name;

/**
 Appends the HTTP header `Content-Disposition: file; filename=#{filename}"` and `Content-Type: #{mimeType}`, followed by the encoded file data and the multipart form boundary.
 
 @param fileURL The URL for the local file to have its contents appended to the form data. This parameter must not be `nil`.
 @param mimeType The MIME type of the specified data. (For example, the MIME type for a JPEG image is image/jpeg.) For a list of valid MIME types, see http://www.iana.org/assignments/media-types/. This parameter must not be `nil`.
 @param fileName The filename to be associated with the file contents. This parameter must not be `nil`.
 @param error If an error occurs, upon return contains an `NSError` object that describes the problem.
 */
- (void)appendPartWithFile:(NSURL *)fileURL mimeType:(NSString *)mimeType fileName:(NSString *)fileName error:(NSError **)error;

/**
 Appends encoded data to the form data.
 
 @param data The data to be encoded and appended to the form data.
 */
- (void)appendData:(NSData *)data;

/**
 Appends a string to the form data.
 
 @param string The string to be encoded and appended to the form data.
 */
- (void)appendString:(NSString *)string;
@end
