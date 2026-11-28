export module MultiControlSamples:Controls.ControlWindow;
import std;
import Win32;
import :Error;

export namespace MultiControlSamples
{
	struct CreateControlArgs
	{
		unsigned ControlId = 0;
		std::wstring ClassName = L"";
		std::wstring Title = L"";
		Win32::DWORD ExtendedStyle = 0;
		Win32::DWORD Style = 0;
		int X = 0;
		int Y = 0;
		unsigned Width = 0;
		unsigned Height = 0;
		Win32::HWND Parent = nullptr;
	};

	auto NextControlId() -> unsigned
	{
		static auto s_nextControlId = 1u;
		return s_nextControlId++;
	}

	struct ControlParams
	{
		std::wstring ClassName;
		Win32::DWORD Style = 0;
		Win32::DWORD ExtendedStyle = 0;
	};

	template<typename T>
	concept ControlBaseType = requires(T t)
	{
		{ t.GetDefaultParams() } -> std::same_as<ControlParams>;
		{ t.HandleMessage(
			std::declval<Win32::HWND>(), 
			std::declval<Win32::UINT>(), 
			std::declval<Win32::WPARAM>(), 
			std::declval<Win32::LPARAM>()) 
		} -> std::same_as<Win32::LRESULT>;
	};

	template<ControlBaseType TDerived>
	class ControlWindow : public TDerived
	{
	public:
		~ControlWindow()
		{
			if (m_hwnd)
				Win32::DestroyWindow(m_hwnd);
		}

		// The subclass callback retains this object's address.
		ControlWindow(const ControlWindow&) = delete;
		auto operator=(const ControlWindow&) -> ControlWindow& = delete;
		ControlWindow(ControlWindow&&) = delete;
		auto operator=(ControlWindow&&) -> ControlWindow& = delete;

		ControlWindow(
			Win32::HWND parent, 
			const std::wstring& title, 
			int x, int y, 
			unsigned width, unsigned height,
			auto&&... args
		) : TDerived{ std::forward<decltype(args)>(args)... }
		{
			auto defaultParams = this->GetDefaultParams();
			Initialize(
				CreateControlArgs{
					.ControlId = NextControlId(),
					.ClassName = defaultParams.ClassName,
					.Title = title,
					.ExtendedStyle = defaultParams.ExtendedStyle,
					.Style = defaultParams.Style,
					.X = x,
					.Y = y,
					.Width = width,
					.Height = height,
					.Parent = parent,
				});
		}

	public:
		[[nodiscard]]
		auto GetHwnd(this auto&& self) noexcept -> Win32::HWND
		{
			return self.m_hwnd;
		}

	protected:
		auto Initialize(this auto&& self, const CreateControlArgs& args) -> decltype(self)
		{
			self.m_hwnd = Win32::CreateWindowExW(
				args.ExtendedStyle,
				args.ClassName.data(),
				args.Title.data(),
				Win32::WindowStyles::Child | Win32::WindowStyles::Visible | args.Style,
				args.X,
				args.Y,
				args.Width,
				args.Height,
				args.Parent,
				reinterpret_cast<Win32::HMENU>(static_cast<Win32::ULONG_PTR>(args.ControlId)),
				Win32::GetModuleHandleW(nullptr),
				&self
			);
			if (not self.m_hwnd)
				throw Error::Win32Error{ Win32::GetLastError(), "Failed to create window" };
			if (not Win32::SetWindowSubclass(self.m_hwnd, ControlProc, 0, reinterpret_cast<Win32::DWORD_PTR>(&self)))
			{
				Win32::DestroyWindow(std::exchange(self.m_hwnd, nullptr));
				throw Error::RuntimeError{ "Failed to subclass control window" };
			}
			return self;
		}

	private:
		static auto ControlProc(
			Win32::HWND hwnd,
			Win32::UINT msg,
			Win32::WPARAM wParam,
			Win32::LPARAM lParam,
			Win32::UINT_PTR subclassId,
			Win32::DWORD_PTR dwRefData
		) noexcept -> Win32::LRESULT
		try
		{
			if (auto ptr = reinterpret_cast<ControlWindow*>(dwRefData); not ptr)
			{
				return Win32::DefSubclassProc(hwnd, msg, wParam, lParam);
			}
			else switch (msg)
			{
				case Win32::WindowMessages::NonClientDestroy:
				{
					ptr->m_hwnd = nullptr;
					Win32::RemoveWindowSubclass(hwnd, ControlProc, subclassId);
					return Win32::DefSubclassProc(hwnd, msg, wParam, lParam);
				}

				default:
				{
					return ptr->HandleMessage(hwnd, msg, wParam, lParam);
				}
			}
		}
		catch (...)
		{
			auto ancestor = Win32::GetAncestor(hwnd, Win32::GetAncestorFlags::Root);
			if (ancestor)
			{
				auto exception = std::current_exception();
				Win32::SendMessageW(
					ancestor,
					Win32::WindowMessages::UnhandledException,
					msg,
					reinterpret_cast<Win32::LPARAM>(&exception)
				);
			}
			return Win32::DefSubclassProc(hwnd, msg, wParam, lParam);
		}

	protected:
		Win32::HWND m_hwnd = nullptr;
	};
}