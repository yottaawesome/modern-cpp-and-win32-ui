export module MultiControlSamples:Font.FontFactory;
import std;
import Win32;
import :Raii;
import :Error;

export namespace MultiControlSamples
{
	template<std::invocable<unsigned> CreateFn>
	struct FontFactory
	{
		constexpr FontFactory(CreateFn fn) noexcept                                              { }

		[[nodiscard]]
		constexpr auto Get(unsigned dpi) const -> FontUniquePtr
		{
			return FontUniquePtr{ std::invoke(CreateFn(), dpi) };
		}

		[[nodiscard]]
		constexpr auto operator()(unsigned dpi) const -> FontUniquePtr
		{
			return Get(dpi);
		}
	};
}
