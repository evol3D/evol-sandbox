this.on_init = function()
  this.speed = 0.1
  this.original_position = this.position
  this.angles = Vec3:new()
  this.mouse_sens = 0.01
  Input.lockCursor()
end

this.on_update = function()
  local deltaMouseMovement = Input.getDeltaMousePos()
  this.angles.x = this.angles.x - deltaMouseMovement.y * this.mouse_sens
  this.angles.y = this.angles.y - deltaMouseMovement.x * this.mouse_sens
  this.eulerAngles = this.angles
end

this.on_fixedupdate = function()
  if Input.getKeyJustPressed(Input.KeyCode.Enter) then
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
