class Movement : Sphynx::Component{
    Movement(){

    }
    void Start()
    {
        Print("Hello From AngelScript");
        Print("Window Width:" + MainWindow.GetWidth());
        Print("Window Height:" + MainWindow.GetHeight());
        MainWindow.SetTitle("Nizar");
        MainWindow.Resize(250,250);
        // MainWindow.Close();
        // App.CreateWindow("New ASWindow",{500,500},false);
    }
    void Update()
    {
        
    }
    void OnDetach(){

    }
}