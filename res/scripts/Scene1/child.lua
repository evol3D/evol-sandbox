this.on_init = function ()
  this.custom_eulerangles = Vec3:new()
  this.custom_angularvelocity = Vec3:new(0, 0.01, 0)
end

this.on_update = function ()
  rb = this:getComponent(Rigidbody)
  if Input.getKeyDown(Input.KeyCode.Enter) then
    rb:addForce(Vec3:new(0, 100, 0))
  end
  if Input.getKeyDown(Input.KeyCode.Right) then
    rb:addForce(Vec3:new(10, 0, 0))
  end
  if Input.getKeyDown(Input.KeyCode.Left) then
    rb:addForce(Vec3:new(-10, 0, 0))
  end
  if Input.getKeyDown(Input.KeyCode.Up) then
    rb:addForce(Vec3:new(0, 0, -10))
  end
  if Input.getKeyDown(Input.KeyCode.Down) then
    rb:addForce(Vec3:new(0, 0, 10))
  end
end
