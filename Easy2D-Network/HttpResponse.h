// Copyright (c) 2016-2018 Easy2D - Nomango
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

#pragma once

namespace easy2d
{
	namespace network
	{
		class HttpResponse
			: public Object
		{
		public:
			inline HttpResponse(HttpRequestPtr const& request)
				: request_(request)
				, succeed_(false)
				, response_code_(0)
			{
			}

			inline HttpRequestPtr const& GetRequest() const
			{
				return request_;
			}

			inline void SetSucceed(bool succeed)
			{
				succeed_ = succeed;
			}

			inline bool IsSucceed() const
			{
				return succeed_;
			}

			inline void SetResponseCode(long response_code)
			{
				response_code_ = response_code;
			}

			inline long GetResponseCode() const
			{
				return response_code_;
			}

			inline void SetHeader(std::string const& response_header)
			{
				response_header_ = response_header;
			}

			inline String GetHeader() const
			{
				return String(response_header_);
			}

			inline std::string* GetHeaderPtr()
			{
				return &response_header_;
			}

			inline void SetData(std::string const& response_data)
			{
				response_data_ = response_data;
			}

			inline String GetData() const
			{
				return String(response_data_);
			}

			inline std::string* GetDataPtr()
			{
				return &response_data_;
			}

			inline void SetError(std::string const& error_buffer)
			{
				error_buffer_ = error_buffer;
			}

			inline String GetError() const
			{
				return String(error_buffer_);
			}

			inline std::string* GetErrorPtr()
			{
				return &error_buffer_;
			}

		protected:
			bool succeed_;
			long response_code_;
			HttpRequestPtr request_;

			std::string response_header_;
			std::string response_data_;
			std::string error_buffer_;
		};
	}
}
