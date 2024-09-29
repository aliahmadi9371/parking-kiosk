@extends('layout.admin')

@section('content')
{{--
    @if (Session::has('flash_message'))
        <div class="alert alert-info" style="background-color: #00FF00">{{ Session::get('flash_message') }}</div>
    @endif
    @if (Session::has('flash_d_message'))
        <div class="alert alert-info" style="background-color: #888888">{{ Session::get('flash_d_message') }}</div>
    @endif--}}
    @if(count($errors))
        <ui class="alert alert--error">
            @foreach ($errors->all() as $error)
                <li>
                    {{ $error }}
                </li>
            @endforeach
        </ui>
    @endif
    <div class="row wrapper border-bottom white-bg page-heading">
        <div class="col-lg-10">
            <h2>تغییر رمز عبور</h2>
            <ol class="breadcrumb">
                <li>
                    <a href="/">خانه</a>
                </li>
                <li class="active">
                    <strong>تغییر رمز عبور</strong>
                </li>
            </ol>
        </div>
        <div class="col-lg-2">
        </div>
    </div>

    <div class="wrapper wrapper-content animated fadeInRight">
        <div class="row">
            <div class="col-lg-12">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>تغییر رمز عبور</h5>
                        <div class="ibox-tools">
                            <a class="collapse-link">
                                <i class="fa fa-chevron-up"></i>
                            </a>
                        </div>
                    </div>
                    <div class="ibox-content">
                        <div class="row">
                            <div style="padding-top: 3%" class="col-sm-8 b-r b-l">
                                <form role="form" id="form" method="POST" action="/change-password" class="form-horizontal">
                                    {{ csrf_field() }}
                                    <input type="hidden" name="_token" value="{{ csrf_token() }}">


                                    <div class="form-group"><label for="presentPassword" class="col-sm-3 control-label">رمز
                                            عبور فعلی</label>
                                        <div class="col-sm-8"><input type="password" name="presentPassword" id="presentPassword" value=""
                                                                     placeholder="******" class="form-control"></div>
                                    </div>
                                    <div class="form-group"><label for="newPassword" class="col-sm-3 control-label">رمز
                                            عبور جدید</label>
                                        <div class="col-sm-8">
                                            <input class="form-control" type="password" id="passwordA" name="newPassword" value=""
                                                   placeholder="******">
                                        </div>
                                    </div>
                                    <div class="form-group"><label for="newPassword_confirmation"
                                                                   class="col-sm-3 control-label">تکرار رمز عبور
                                            جدید</label>
                                        <div class="col-sm-8">
                                            <input class="form-control" type="password" name="newPassword_confirmation"
                                                   value="" placeholder="******">
                                        </div>
                                    </div>


                                    <div class="form-group">
                                        <div class="col-sm-4 col-sm-offset-3">
                                            <button class="btn btn-primary" type="submit"> تغییر رمز عبور</button>
                                        </div>
                                    </div>

                                </form>
                            </div>
                            <div class="col-sm-4">
                                <div style="  background-image: url(/img/admin/key.png);
                                 background-size: contain; margin-left: auto; padding: 0; background-position: center center;
                                 background-repeat: no-repeat;  height: 85px; width:58%;     margin-right: 21%;
                                 margin-top: 20%;">

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
                    presentPassword: {
                        required: true,
                    },
                    newPassword: {
                        required: true,
                        minlength: 6
                    },
                    newPassword_confirmation: {
                        required: true,
                        minlength: 6,
                        equalTo: "#passwordA"
                    },

                }
            });
        });





    </script>
@stop