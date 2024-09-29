<?php

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| contains the "web" middleware group. Now create something great!
|
*/

Route::get('/', function () {
    return view('welcome');
});

Auth::routes();

Route::get('/', 'HomeController@homePage');
Route::get('/home', 'HomeController@homePage');
Route::post('/change-password', 'HomeController@storeNewPassword');


Route::get('/network-setting', 'HomeController@networkPage');
Route::get('/reset', 'HomeController@reset');



Route::post('/network-setting', 'HomeController@changeNetworkSetting');

Route::get('/server-setting', 'HomeController@serverPage');
Route::post('/server-setting', 'HomeController@changeServerSetting');

Route::get('logout', '\App\Http\Controllers\Auth\LoginController@logout');
