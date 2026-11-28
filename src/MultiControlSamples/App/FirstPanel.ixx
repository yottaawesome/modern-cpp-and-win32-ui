export module MultiControlSamples:App.FirstPanel;
import stdx;
import Win32;
import :Panel;

export namespace MultiControlSamples
{
	class FirstPanel
	{
	public:
		FirstPanel(Win32::HWND parent)
			: m_parent{ parent }
		{

		}

		auto ApplyLayout()
		{

		}

		auto Show(this auto&& self) -> decltype(self)
		{
			self.m_panel.Show();
			return self;
		}

		auto Hide(this auto&& self) -> decltype(self)
		{
			self.m_panel.Hide();
			return self;
		}

	private:
		Win32::HWND m_parent = nullptr;
		PanelWindow m_panel{
			CustomWindowArgs{
				.X = 0,
				.Y = 0,
				.Width = 800,
				.Height = 500,
				.Parent = m_parent
			}
		};
	};
}