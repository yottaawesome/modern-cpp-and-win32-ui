export module MultiControlSamples:App.AllPanels;
import stdx;
import Win32;
import :App.FirstPanel;

export namespace MultiControlSamples
{
	class AllPanels
	{
	public:
		AllPanels(Win32::HWND parent)
			: m_parent{ parent }
		{
			SwitchToPanel<FirstPanel>(m_parent);
		}

		template<typename TPanel>
		auto SwitchToPanel(this auto&& self, auto&&...args) -> decltype(self)
		{
			auto newPanel = std::make_unique<TPanel>(std::forward<decltype(args)>(args)...);
			self.m_currentPanel.Visit(
				[](std::monostate) {},
				[](auto&& panel) { panel->Hide(); }
			);
			newPanel->Show();
			self.m_currentPanel.Emplace<std::unique_ptr<TPanel>>(std::move(newPanel));
			return self;
		}

	private:
		Win32::HWND m_parent = nullptr;
		stdx::Variant<std::monostate, std::unique_ptr<FirstPanel>> m_currentPanel;
	};
}
