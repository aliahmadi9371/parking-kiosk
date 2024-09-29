<!DOCTYPE html>
<html>
<head>
    <meta name="csrf-token" content="{{ csrf_token() }}">
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    @yield('title')

    <meta name="language" content="Fa-IR" />
    <meta name="geo.region" content="IR" />{{--  google iran ra hadaf gharar dahad --}}
    <meta name="geo.position" content="32.427908;53.688046" />{{--  google iran ra hadaf gharar dahad --}}
    <meta name="ICBM" content="32.427908, 53.688046" />{{--  google iran ra hadaf gharar dahad --}}

    <link rel="stylesheet" href="/css/admin/base_css_admin/admin_base_css.css">
    <link rel="stylesheet" href="/css/admin/style.css">
    <link rel="stylesheet" href="/css/admin/plugin/toastr/toastr.min.css">
    <link rel="stylesheet" href="/css/admin/plugin/iCheck/custom.css">
    @yield('head')


</head>

<body class="rtls ">





<div id="wrapper">

    @include('includes.adminSidebar')

    <div id="page-wrapper" class="gray-bg" {{--style=" margin: 0 70px 0 0 !important;"--}}> {{--saeedi--}}

        <div class="row border-bottom">
            <nav class="navbar navbar-static-top" role="navigation" style="margin-bottom: 0">
                <div class="navbar-header">
                    <a class="navbar-minimalize minimalize-styl-2 btn btn-primary " href="#"><i class="fa fa-bars"></i> </a>
                    <form role="search" class="navbar-form-custom" action="search_results.html">
                        <div class="form-group">
                            {{--<input type="text" placeholder="Search for something..." class="form-control" name="top-search" id="top-search">
                       --}} </div>
                    </form>
                </div>
                <ul class="nav navbar-top-links navbar-right">
                    <li>
                        {{--<span class="m-r-sm text-muted welcome-message">Welcome to INSPINIA+ Admin Theme.</span>--}}
                    </li>
                    <li>
                        <a href="/reset">
                            <i class="fa fa-power-off"></i>  راه اندازی مجدد
                        </a>
                    </li>

                    <li>
                        <a href="/">
                            <i class="fa fa-home"></i> صفحه اصلی
                        </a>

                    </li>
                    <li>
                        <a href="/logout">
                            <i class="fa fa-sign-out"></i> خروج
                        </a>
                    </li>
                    <li>
                        <a class="right-sidebar-toggle">
                            <i class="fa fa-tasks"></i>
                        </a>
                    </li>
                </ul>
            </nav>
        </div>

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
</div>









<script src="/js/admin/base_js/base_admin_js.js"></script>
<script src="/js/admin/plugin/metisMenu/jquery.metisMenu.js"></script>
<script src="/js/admin/plugin/slimscroll/jquery.slimscroll.min.js"></script>{{--plugin--}}


<script src="/js/admin/plugin/inspinia.js"></script>
<script src="/js/admin/plugin/pace/pace.min.js"></script>
<script src="/js/admin/plugin/toastr/toastr.min.js"></script>
<script src="/js/admin/plugin/iCheck/icheck.min.js"></script>


<script>
    $(document).ready(function() {
        $('.i-checks').iCheck({
            checkboxClass: 'icheckbox_square-green',
            radioClass: 'iradio_square-green'
        });

        toastr.options = {
            "closeButton": false,
            "progressBar": true,
        };;


        @if(session()->has('flash_message'))
                       toastr.success("{{ @session('flash_message') }}");

        @endif
        @if(session()->has('flash_d_message'))
                toastr.warning("{{ @session('flash_d_message') }}");

        @endif

    });



</script>


{{--<!-- Peity -->
<script src="/js/plugins/peity/jquery.peity.min.js"></script>
<script src="/js/demo/peity-demo.js"></script>--}}


@yield('scripts')
</body>
</html>