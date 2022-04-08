class Camera : Sphynx::Component{
    float FOV{
            get{
                return GetFOV();
            }
            set{
                SetFOV(value);
            }
    }
    void Update()
    {
    }
    void OnDetach(){
    }
}