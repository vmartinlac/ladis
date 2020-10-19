
import wsgiref.simple_server
import pyramid.config
import views
import filters

if __name__ == '__main__':

    cfg = pyramid.config.Configurator()

    cfg.include('pyramid_jinja2')

    def setup_jinja2():
        jinja_env = cfg.get_jinja2_environment()
        jinja_env.filters['filter_datetime'] = filters.filter_datetime
        jinja_env.filters['filter_first_player'] = filters.filter_first_player
        jinja_env.filters['filter_result'] = filters.filter_result
        jinja_env.filters['filter_num_moves'] = filters.filter_num_moves
        jinja_env.filters['filter_time_offset'] = filters.filter_time_offset
    cfg.action(None, setup_jinja2, order=999)

    cfg.add_route('index', '/')
    cfg.add_route('match', '/match/{match}')
    cfg.add_route('static', '/static')

    cfg.add_static_view(name='static', path=views.static_directory)

    cfg.scan(views)

    app = cfg.make_wsgi_app()

    server = wsgiref.simple_server.make_server('0.0.0.0', 8080, app)
    server.serve_forever()

