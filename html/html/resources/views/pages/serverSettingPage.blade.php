@extends('layout.admin')


@section('title')
    <title>تنظیمات سرور</title>
@stop
<style>

   button {
        float: left;
    }

    @media (min-width: 768px) {
        .{
            float: left !important;
        }

    }

</style>


@section('content')
    {{-- @if (Session::has('flash_message'))
         <div class="alert alert-info" style="background-color: #00FF00">{{ Session::get('flash_message') }}</div>
     @endif--}}
    <div style=" padding-bottom: 10%;" class="container-fluid ">


        <div class="wrapper wrapper-content animated fadeInRight">

            <div class="row">
                <div class="col-md-8 col-md-offset-2 col-sm-8 col-sm-offset-2">
                    <div class="ibox float-e-margins">
                        <div class="ibox-title">
                            <h5 >تنظیمات</h5>
                        </div>
                        <div class="ibox-content">
                            <div class="row">
                                <div style="padding-top: 3%;" class="col-sm-12 ">
                                    <form role="form" id="form" method="POST" action="/server-setting"
                                          class="form-horizontal">
                                        {{ csrf_field() }}
                                        <input type="hidden" name="_token" value="{{ csrf_token() }}">

                                        <div class="form-group"><label for="subnetMask"
                                                                       class="col-sm-3 control-label">شماره دستگاه
                                            </label>
                                            <div class="col-sm-8">
                                                <input class="form-control" type="text" id="deviceId"
                                                       name="deviceId" value="{{$deviceId}}"
                                                >
                                            </div>
                                        </div>
                                        <div class="form-group"><label for="gateWay"
                                                                       class="col-sm-3 control-label">پیام نمایش داده شده در هنگام غیر فعال بودن لوپ
                                            </label>
                                            <div class="col-sm-8">
                                                <input class="form-control" type="text" name="disabled_loop_meesage"
                                                       id="disabled_loop_meesage"
                                                       value="{{$disabled_loop_meesage}}">
                                            </div>
                                        </div>
                                        <div class="form-group"><label for="gateWay"
                                                                       class="col-sm-3 control-label">فعال سازی لوپ
                                            </label>
                                            <div class="col-sm-8">

                                                <div class="i-checks" style="display: inline-block" >
                                                    <label>
                                                        <input class="form-control check" type="checkbox" name="loop_enable" {{$loop_enable ? 'checked' : ''}}>
                                                    </label>
                                                </div>


                                                {{--<input class="form-control" type="text" name="loop_enable"
                                                       id="loop_enable"
                                                       value="{{$loop_enable}}">--}}
                                            </div>
                                        </div>
                                        <div class="form-group"><label for="gateWay"
                                                                       class="col-sm-3 control-label">پیام نمایش داده شده در هنگام فعال بودن لوپ

                                            </label>
                                            <div class="col-sm-8">
                                                <input class="form-control" type="text" name="message" id="message"
                                                       value="{{$message}}">
                                            </div>
                                        </div>


                                        <div class="form-group"><label for="dns"
                                                                       class="col-sm-3 control-label"> پیام نمایش داده شده برای برداشتن قبض

                                            </label>
                                            <div class="col-sm-8">
                                                <input class="form-control" type="text" name="receiptMessage"
                                                       id="receiptMessage"
                                                       value="{{ $receiptMessage }}">
                                            </div>
                                        </div>
                                        <div class="form-group"><label for="dns"
                                                                       class="col-sm-3 control-label">پیام برای کارت مجاز

                                            </label>
                                            <div class="col-sm-8">
                                                <input class="form-control" type="text" name="cardMessage"
                                                       id="cardMessage"
                                                       value="{{ $cardMessage }}">
                                            </div>
                                        </div>
                                        <div class="form-group"><label for="dns"
                                                                       class="col-sm-3 control-label">پیام برای کارت غیر مجاز

                                            </label>
                                            <div class="col-sm-8">
                                                <input class="form-control" type="text" name="cardMessageNot"
                                                       id="cardMessageNot"
                                                       value="{{ $cardMessageNot }}">
                                            </div>
                                        </div>
                                        <div class="form-group"><label for="dns"
                                                                       class="col-sm-3 control-label"> زمان انجام پروسه
                                            </label>
                                            <div class="col-sm-8">
                                                <input class="form-control" type="text" name="tLCD" id="tLCD"
                                                       value="{{ $tLCD }}">
                                            </div>
                                        </div>

                                        <div class="form-group"><label for="dns"
                                                                       class="col-sm-3 control-label"> وب سرویس ۱
                                            </label>
                                            <div class="col-sm-8">
                                                <input class="form-control" type="text" name="webservice1"
                                                       id="webservice1"
                                                       value="{{ $webservice1 }}">
                                            </div>
                                        </div>

                                        <div class="form-group"><label for="dns"
                                                                       class="col-sm-3 control-label"> وب سرویس ۲

                                            </label>
                                            <div class="col-sm-8">
                                                <input class="form-control" type="text" name="webservice2"
                                                       id="webservice2"
                                                       value="{{ $webservice2 }}">
                                            </div>
                                        </div>
                                        <div class="form-group"><label for="dns"
                                                                       class="col-sm-3 control-label"> وب سرویس ۳

                                            </label>
                                            <div class="col-sm-8">
                                                <input class="form-control" type="text" name="webservice3"
                                                       id="webservice3"
                                                       value="{{ $webservice3 }}">
                                            </div>
                                        </div>

                                        <div class="form-group"><label for="dns"
                                                                       class="col-sm-3 control-label"> آدرس و پورت سرور
                                            </label>
                                            <div class="col-sm-8">
                                                <input class="form-control" type="text" name="baseurl" id="baseurl"
                                                       value="{{ $baseurl }}">
                                            </div>
                                        </div>


                                        <div class="form-group"><label
                                                    class="col-sm-3 control-label">
                                            </label>
                                            <div class="col-sm-8">
                                                <button class="btn btn-primary" type="submit">ثبت اطلاعات
                                                </button>
                                            </div>
                                        </div>
                                    </form>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>

@stop


@section('scripts')

    <script>


        $(document).ready(function () {

            $("#form").validate({
                rules: {
                    deviceId: {
                        required: true,

                    },
                    disabled_loop_meesage: {
                        required: true,

                    },
                    message: {
                        required: true,

                    },
                    receiptMessage: {
                        required: true,

                    },
                    cardMessage: {
                        required: true,

                    },
                    cardMessageNot: {
                        required: true,

                    },
                    tLCD: {
                        required: true,

                    },
                    webservice1: {
                        required: true,

                    },
                    webservice2: {
                        required: true,

                    },
                    webservice3: {
                        required: true,

                    },
                    baseurl: {
                        required: true,

                    },



                }

            });

        });

    </script>





@stop