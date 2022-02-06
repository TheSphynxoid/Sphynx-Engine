class Movement : Sphynx::Component{
    Movement(){

    }
    void Start()
    {
        Print("Hello From AngelScript");
        Print("Window Width:" + MainWindow.GetWidth());
        Print("Window Height:" + MainWindow.GetHeight());
    }
    void Update()
    {
      Print("AsScript");  
    }
    void OnDetach(){

    }
}