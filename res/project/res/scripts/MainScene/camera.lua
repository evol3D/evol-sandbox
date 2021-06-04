this.on_init = function()
  this.speed = 0.1
  this.original_position = this.position
end

this.on_fixedupdate = function()
  if Input.getKeyDown(Input.KeyCode.Enter) then
    gotoScene('SideScene')
  end

  if Input.getKeyDown(Input.KeyCode.Up) then
    this.position = this.position + Vec3:new(0, 1, 0) * this.speed
  end
  if Input.getKeyDown(Input.KeyCode.Down) then
    this.position = this.position - Vec3:new(0, 1, 0) * this.speed
  end
  if Input.getKeyDown(Input.KeyCode.Right) then
    this.position = this.position + Vec3:new(1, 0, 0) * this.speed
  end
  if Input.getKeyDown(Input.KeyCode.Left) then
    this.position = this.position - Vec3:new(1, 0, 0) * this.speed
  end
end
