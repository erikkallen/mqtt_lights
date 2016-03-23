wpi = require("wiring-pi")
Kaku = require("./kaku.coffee")
mqtt    = require('mqtt')
lights = require("./config.json")

client  = mqtt.connect('mqtt://localhost')

map = (x, in_min, in_max, out_min, out_max) ->
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min


console.log lights
lights = lights.lights
for light in lights
  light.kaku = new Kaku(wpi,light.address,1,260,6)

  light.kaku.sendDim(light.device,15)
  light.kaku.sendUnit(light.device,false)




client.on 'connect', () ->
  console.log "Connected"
  for light in lights
    #console.log light
    #client.subscribe(light.state_topic)
    client.subscribe(light.command_topic) if light.command_topic
    client.subscribe(light.brightness_topic) if light.brightness_topic
    client.publish(light.state_topic, "off",{retain:true}) if light.state_topic
    client.publish(light.brightness_state_topic, "255",{retain:true}) if light.brightness_state_topic


client.on 'message', (topic, message) ->
  # message is Buffer
  console.log(message.toString())
  console.log(topic)
  for light in lights
    if topic == light.command_topic
      if message.toString() == "on"
        res = true
      else
        res = false
      console.log "Send: "+res+" dev: "+ light.device
      light.kaku.sendUnit(light.device,res)
      client.publish(light.state_topic, message)
    if topic == light.brightness_topic
      val = parseInt(message.toString())
      val = map(val,0,255,1,15)
      console.log "Send: "+Math.round(val)+" dev: "+ light.device
      light.kaku.sendDim(light.device,Math.round(val)+"")
      client.publish(light.brightness_state_topic, message)
