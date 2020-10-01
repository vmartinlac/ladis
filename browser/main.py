
import wsgiref.simple_server
import pyramid.config

if __name__ == '__main__':
    cfg = pyramid.config.Configurator()
    cfg.add_route('index', '/')
    cfg.add_route('match', '/match/{match}')
    cfg.include('pyramid_jinja2')
    cfg.scan('views')
    app = cfg.make_wsgi_app()
    server = wsgiref.simple_server.make_server('0.0.0.0', 8080, app)
    server.serve_forever()

