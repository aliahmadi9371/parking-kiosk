let mix = require('laravel-mix');

/*
 |--------------------------------------------------------------------------
 | Mix Asset Management
 |--------------------------------------------------------------------------
 |
 | Mix provides a clean, fluent API for defining some Webpack build steps
 | for your Laravel application. By default, we are compiling the Sass
 | file for the application as well as bundling up all the JS files.
 |
 */

mix.js('resources/assets/js/app.js', 'public/js')
    .sass('resources/assets/sass/app.scss', 'public/css')


    .styles([
        'resources/assets/css/admin/base_css/bootstrap.min.css',
        'resources/assets/css/admin/base_css/bootstrap-rtl.css',
        'resources/assets/css/admin/base_css/font-awesome.css',
        'resources/assets/css/admin/plugin/toastr/toastr.min.css',
        'resources/assets/css/admin/animate.css',


    ], 'public/css/admin/base_css_admin/admin_base_css.css')

    .copyDirectory([
        'resources/assets/css/admin/plugin',
    ], 'public/css/admin/plugin')

    .copyDirectory([
        'resources/assets/fonts',
    ], 'public/fonts')

    .sass('resources/assets/sass/admin/style.scss', 'public/css/admin/style.css')

    .scripts([
        'resources/assets/js/admin/base_js/jquery-2.1.1.js',
        'resources/assets/js/admin/base_js/bootstrap.min.js',
        'resources/assets/js/admin/plugin/validate/jquery.validate.min.js'
        /*'resources/assets/js/admin/base_js/inspinia.js',
         'resources/assets/js/admin/base_js/pace.min.js',*/
    ], 'public/js/admin/base_js/base_admin_js.js')

    .scripts([/*saeedi*/
        'resources/assets/js/admin/plugin/afterglow/afterglow.min.js'
    ], 'public/js/admin/plugin/afterglow/afterglow.min.js')

    .scripts([/*saeedi*/
        'resources/assets/js/admin/plugin/ckeditor/ckeditor.js'
    ], 'public/js/admin/plugin/ckeditor/ckeditor.js')

    .scripts([/*saeedi*/
        'resources/assets/js/admin/plugin/ckeditor/sample.js'
    ], 'public/js/admin/plugin/ckeditor/sample.js')


    .copyDirectory([
        'resources/assets/js/admin/plugin'
    ], 'public/js/admin/plugin')

    .copyDirectory([
        'resources/assets/js/admin/plugin/ckeditor'
    ], 'public/js/admin/plugin/ckeditor')

    .copyDirectory([
        'resources/assets/css/admin/plugin'
    ], 'public/css/admin/plugin')

    .copyDirectory([
        'resources/assets/img/admin',
    ], 'public/img/admin');
