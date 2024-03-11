#ifndef __graph_basic_string_hpp


namespace Graph::Standart
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