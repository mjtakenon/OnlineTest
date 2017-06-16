# include <Siv3D.hpp>
# include <HamFramework.hpp>

# include <vector>

using namespace std;

class Title : public SceneManager<String>::Scene
{
public:

	void init() override
	{

	}

	void update() override
	{
		if (Input::MouseL.clicked && Mouse::Pos().x < Window::Width() / 2)
			changeScene(L"Server");
		else if (Input::MouseL.clicked && Mouse::Pos().x > Window::Width() / 2)
			changeScene(L"Client");
	}

	void draw() const override
	{
		Window::ClientRect().draw(Palette::Black);
		Rect(0,0,Window::Width()/2,Window::Height()).draw(Palette::Black);
		font(L"Server/Client").drawCenter(Window::Center());
	}

	Font font{ 50 };
};

class Server : public SceneManager<String>::Scene
{
public:
	void init() override
	{
		server.startAccept(58620);
	}

	void update() override
	{
		if (Input::MouseL.clicked)
			changeScene(L"Title");

		if (server.isConnected())
		{
			while (server.read(receivedPoint));
		}

		if (server.hasError())
		{
			server.disconnect();

			server.startAccept(58620);
		}

	}

	void draw() const override
	{
		Window::ClientRect().draw(Palette::Black);
		font(L"Server").drawCenter(Window::Center());

		Circle(receivedPoint, 10).draw(Palette::Orange);
	}

	Font font{ 50 };

	TCPServer server;

	Point receivedPoint;
};

class Client : public SceneManager<String>::Scene
{
public:

	void init()
	{
		String str(L"127.0.0.1");
		vector<String> sstr = str.split('.');
		client.connect(IPv4(FromString<int>(sstr[0], 10), 
							FromString<int>(sstr[1], 10), 
							FromString<int>(sstr[2], 10), 
							FromString<int>(sstr[3], 10)), 58620);
	}

	void update() override
	{
		if (Input::MouseL.clicked)
			changeScene(L"Title");


		if (client.isConnected())
		{
			client.send(Mouse::Pos());
		}

		if (client.hasError())
		{
			client.disconnect();

			client.connect(IPv4::localhost(), 58620);
		}

	}

	void draw() const override
	{
		Window::ClientRect().draw(Palette::Black);
		font(L"Client").drawCenter(Window::Center());
	}

	Font font{ 50 };

	TCPClient client;
	
};

void Main()
{
	SceneManager<String> manager;

	manager.add<Title>(L"Title");
	manager.add<Server>(L"Server");
	manager.add<Client>(L"Client");

	while (System::Update())
	{
		manager.updateAndDraw();
	}
}
