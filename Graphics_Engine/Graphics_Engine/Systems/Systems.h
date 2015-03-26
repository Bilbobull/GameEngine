// Author: Gonzalo Rojo

class System
{
public:
  virtual void Init(void) = 0;
  virtual void Update(double dt) { if (dt) return; return; }
  virtual void Free(void) = 0;
private:
  double dt;
};

