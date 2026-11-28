export module MultiControlSamples:Font.FontLoader;
import std;
import Win32;
import :Raii;
import :Error;

export namespace MultiControlSamples
{
	template<std::invocable CreateFn>
	class FontLoader
	{
	public:
		constexpr FontLoader(CreateFn fn)
		{ }

		[[nodiscard]]
		constexpr auto Get() const -> Win32::HFONT
		{
			std::call_once(
				m_fontInitialised, 
				[this] { m_font = FontUniquePtr{ std::invoke(CreateFn()) }; });
			return m_font.get();
		}

		[[nodiscard]]
		constexpr operator Win32::HFONT() const
		{
			return Get();
		}

	protected:
		mutable std::once_flag m_fontInitialised;
		mutable FontUniquePtr m_font;
	};
}
