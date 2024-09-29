@extends('layout.admin')


@section('title')
    <title>تنظیمات شبکه</title>
@stop
<style>
    .btn-circle {
        width: 20px !important;
        height: 20px !important;
        font-size: 6px !important;
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
                            <h5 >تنظیمات شبکه</h5>
                        </div>
                        <div class="ibox-content">
                            <div class="row">
                                <div style="padding-top: 3%;" class="col-sm-12 ">
                                    <form role="form" id="form" method="POST" action="/network-setting" class="form-horizontal">
                                        {{ csrf_field() }}
                                        <input type="hidden" name="_token" value="{{ csrf_token() }}">

                                        <div class="form-group"><label  for="ipAddress"
                                                                       class="col-sm-3 control-label">Ip Address
                                            </label>
                                            <div class="col-sm-8"><input type="text" name="ipAddress"
                                                                                 id="ipAddress" value="{{$ipAddress}}"
                                                                                 class="form-control"></div>
                                        </div>
                                        <div class="form-group"><label  for="subnetMask"
                                                                       class="col-sm-3 control-label">Subnet
                                                Mask
                                            </label>
                                            <div class="col-sm-8">
                                                <input class="form-control" type="text" id="subnetMask"
                                                       name="subnetMask" value="{{$subnetMask}}"
                                                >
                                            </div>
                                        </div>
                                        <div class="form-group"><label  for="gateWay"
                                                                       class="col-sm-3 control-label">Gate Way
                                            </label>
                                            <div class="col-sm-8">
                                                <input class="form-control" type="text" name="gateWay" id="gateWay"
                                                       value="{{$gateWay}}">
                                            </div>
                                        </div>

                                        <div class="form-group"><label for="dns"
                                                                       class="col-sm-3 control-label">DNS-NameServer
                                            </label>
                                            <div class="col-sm-8">
                                                <input class="form-control" type="text" name="dns_nameservers" id="dns_nameservers"
                                                       value="{{$dns_nameservers}}">
                                            </div>
                                        </div>

                                        <div class="form-group"><label
                                                                       class="col-sm-3 control-label">
                                            </label>
                                            <div class="col-sm-8">
                                                <button style="float: left;" class="btn btn-primary" type="submit">ثبت اطلاعات
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
                    ipAddress: {
                        required: true,

                    },
                    subnetMask: {
                        required: true,

                    },
                    gateWay: {
                        required: true,

                    },
                    dns_nameservers: {
                        required: true,

                    }

                }

            });

        });

    </script>





@stop