#ifndef RUBY_BASIC_STRING_HPP
#define RUBY_BASIC_STRING_HPP


namespace Ruby::Standart
{
	template<typename CharType>
	class BasicString
	{
	public:


	private:
		size_t m_capacity = 0;
		CharType* m_data = nullptr;
	};


	using String = BasicString<char>;
	using WString = BasicString<wchar_t>;
}

#endif // RUBY_BASIC_STRING_HPP