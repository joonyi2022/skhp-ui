# skhp-ui
## dotfiles
`dotfiles/obj` and `dotfiles/moc` is backup of `skhp-ui/.obj` and `skhp-ui/.moc`.
When users run `make clean && make`, `.obj/*` and `.moc/*` should be created at project home, but sometimes there is a chance to fail. So I backup it up at `dotfiles` directory

## MQTT
1. File `use_https` exist in `/usr/local`
2. File `hostnamehttps` exist in `/usr/local` and the content is, for example
```
skhoffice2.ngork.io
skhoffice2.ngork.io
29984
```
Above three lines represent
```
m_http_host_name
m_mqtt_host_name
m_mqtt_host_port
```
