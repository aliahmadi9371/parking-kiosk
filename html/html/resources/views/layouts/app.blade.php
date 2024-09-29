<!DOCTYPE html>
<html lang="{{ app()->getLocale() }}">
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" type="image/png" sizes="32x32" href="/img/admin/favicon/favicon 16x16.png">
    <link rel="icon" type="image/png" sizes="16x16" href="/img/admin/favicon/favicon 32x32.png">
    <link rel="icon" type="image/png" sizes="16x16" href="/img/admin/favicon/favicon 64x64.png">
    <!-- CSRF Token -->
    <meta name="csrf-token" content="{{ csrf_token() }}">

    <title>آساراد</title>

    <!-- Styles -->
    <link href="{{ asset('css/app.css') }}" rel="stylesheet">
    <link rel="stylesheet" href="/css/admin/base_css_admin/admin_base_css.css">
    <link rel="stylesheet" href="/css/admin/style.css">
</head>
<body>
    <div id="app">
        <nav class="navbar navbar-default navbar-static-top">
            <div class="container">
                <div class="navbar-header" style="float: right">


                    <!-- Branding Image -->
                    <a class="navbar-brand" href="{{ url('/') }}">
                        آساراد
                    </a>
                </div>

                <div class="collapse navbar-collapse" id="app-navbar-collapse">
                    <!-- Left Side Of Navbar -->
                    <ul class="nav navbar-nav">
                        &nbsp;
                    </ul>

                    <!-- Right Side Of Navbar -->
                    <ul class="nav navbar-nav navbar-right">
                        <!-- Authentication Links -->
                    </ul>
                </div>
            </div>
        </nav>

        @yield('content')
        <div class="footer">
            <div class="pull-right">
                {{--   10GB of <strong>250GB</strong> Free.--}}
            </div>
            <div>
                1398&copy;   تمام حقوق این سایت متعلق به  <strong><a target="_blank" href="http://asarad.ir/">شرکت آساراد</a></strong>  می باشد.

            </div>
        </div>
    </div>


    <!-- Scripts -->
    <script src="{{ asset('js/app.js') }}"></script>
</body>
</html>
