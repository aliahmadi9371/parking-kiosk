<?php

namespace App\Http\Controllers;

use App\User;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Session;

class HomeController extends Controller
{
    /**
     * Create a new controller instance.
     *
     * @return void
     */
    public function __construct()
    {
        $this->middleware('auth');
    }

    public function homePage()
    {
        return view('pages.homePage');
    }

    public function storeNewPassword(Request $request)
    {
        $users = new User();
        $user = $users->currentUser();
        $presentPassword = $request['presentPassword'];

        if (password_verify($presentPassword, $user->password)) {
            $user->update(array(
                'password' => bcrypt($request['newPassword']),
            ));

            return Redirect('/')->with(Session::flash('flash_message', 'رمز عبور جدید با موفقیت تغییر یافت.'));

        } else {
            return Redirect('/')->with(Session::flash('flash_d_message', 'رمز عبور وارد شده با رمز عبور فعلی مطابقت ندارد.'));

        }

    }

    public function networkPage()
    {
        $dns_nameservers = file_get_contents(public_path("/networkSetting/dns_nameservers.txt"));
        $ipAddress = file_get_contents(public_path("/networkSetting/ipAddress.txt"));
        $subnetMask = file_get_contents(public_path("/networkSetting/subnetMask.txt"));
        $gateWay = file_get_contents(public_path("/networkSetting/gateWay.txt"));
        return view('pages.networkSettingPage', compact('dns_nameservers', 'ipAddress', 'subnetMask', 'gateWay'));
    }


    public function changeNetworkSetting(Request $request)
    {


        $netFile = public_path("/networkSetting/interfaces");
        $newNet = '
auto lo
iface lo inet loopback


#My IP description
# IPv4 address
auto eth0
iface eth0 inet static
    address ' . $request['ipAddress'] . '
    netmask ' . $request['subnetMask'] . '
    gateway ' . $request['gateWay'] . '
    dns-nameservers ' . $request['dns_nameservers'];
        file_put_contents($netFile, $newNet);


        $dnsFile = public_path("/networkSetting/dns_nameservers.txt");
        $newDns = $request['dns_nameservers'];
        file_put_contents($dnsFile, $newDns);

        $gatewayFile = public_path("/networkSetting/gateWay.txt");
        $newGateway = $request['gateWay'];
        file_put_contents($gatewayFile, $newGateway);

        $subnetFile = public_path("/networkSetting/subnetMask.txt");


        $newSubnet = $request['subnetMask'];
        file_put_contents($subnetFile, $newSubnet);

        $ipFile = public_path("/networkSetting/ipAddress.txt");
        $newIp = $request['ipAddress'];
        file_put_contents($ipFile, $newIp);
        exec(public_path("/networkSetting/net.sh"));

        return Redirect('/network-setting')->with(Session::flash('flash_message', 'اطلاعات شبکه با موفقیت ویرایش شد.'));

    }

    public function serverPage()
    {
        $deviceId = file_get_contents(public_path("/serverSetting/deviceId.txt"));
        $disabled_loop_meesage = file_get_contents(public_path("/serverSetting/disabled_loop_meesage.txt"));
        $message = file_get_contents(public_path("/serverSetting/message.txt"));
        $cardMessage = file_get_contents(public_path("/serverSetting/cardMessage.txt"));

        $receiptMessage = file_get_contents(public_path("/serverSetting/receiptMessage.txt"));
        $cardMessageNot = file_get_contents(public_path("/serverSetting/cardMessageNot.txt"));

        $tLCD = file_get_contents(public_path("/serverSetting/tLCD.txt"));
        $webservice1 = file_get_contents(public_path("/serverSetting/webservice1.txt"));
        $webservice2 = file_get_contents(public_path("/serverSetting/webservice2.txt"));
        $webservice3 = file_get_contents(public_path("/serverSetting/webservice3.txt"));
        $loop_enable = file_get_contents(public_path("/serverSetting/loop_enable.txt"));

        $baseurl = file_get_contents(public_path("/serverSetting/baseurl.txt"));


        return view('pages.serverSettingPage', compact('deviceId', 'disabled_loop_meesage', 'message',
            'receiptMessage', 'tLCD', 'webservice1', 'webservice2', 'baseurl', 'cardMessage', 'cardMessageNot', 'webservice3', 'loop_enable'
        ));
    }

    public function reset(Request $request)
    {
        exec(public_path("/networkSetting/reboot.sh"));
        exit();
    }


    public function changeServerSetting(Request $request)
    {
//        dd($request);
        $netFile = public_path("/networkSetting/Saman_config.ini");
        $file_lines = file($netFile);
        $newString = '';
        foreach ($file_lines as $line) {

            if (strpos($line, 'device_id=') !== false) {
                $newString = $newString . 'device_id=' . $request['deviceId'] . "\r\n";
            } elseif (strpos($line, 'disabled_loop_meesage=') !== false) {
                $newString = $newString . 'disabled_loop_meesage=' . $request['disabled_loop_meesage'] . "\r\n";
            } elseif (strpos($line, 'message=') !== false && substr($line, 0, 7) === 'message') {
                $newString = $newString . 'message=' . $request['message'] . "\r\n";
            } elseif (strpos($line, 'receiptMessage=') !== false) {
                $newString = $newString . 'receiptMessage=' . $request['receiptMessage'] . "\r\n";
            } elseif (strpos($line, 'cardMessage=') !== false) {
                $newString = $newString . 'cardMessage=' . $request['cardMessage'] . "\r\n";
            } elseif (strpos($line, 'cardMessageNot=') !== false) {
                $newString = $newString . 'cardMessageNot=' . $request['cardMessageNot'] . "\r\n";
            } elseif (strpos($line, 'tLCD=') !== false) {
                $newString = $newString . 'tLCD=' . $request['tLCD'] . "\r\n";
            } elseif (strpos($line, 'webservice1=') !== false) {
                $newString = $newString . 'webservice1=' . $request['webservice1'] . "\r\n";
            } elseif (strpos($line, 'webservice2=') !== false) {
                $newString = $newString . 'webservice2=' . $request['webservice2'] . "\r\n";
            } elseif (strpos($line, 'webservice3=') !== false) {
                $newString = $newString . 'webservice3=' . $request['webservice3'] . "\r\n";
            } elseif (strpos($line, 'baseurl=') !== false) {
                $newString = $newString . 'baseurl=' . $request['baseurl'] . "\r\n";
            } elseif (strpos($line, 'loop_enable=') !== false) {
                if( $request['loop_enable'] == 'on'){
                    $loop_enable =  'true';
                } else {
                    $loop_enable =  'false';
                }

                $newString = $newString . 'loop_enable=' . $loop_enable . "\r\n";
            } else {
                $newString = $newString . $line;
            }
        }

        file_put_contents($netFile, $newString);

        $variables = array(
            'deviceId',
            'disabled_loop_meesage',
            'message',
            'cardMessage',
            'receiptMessage',
            'cardMessageNot',
            'tLCD',
            'webservice1',
            'webservice2',
            'webservice3',
            'loop_enable',
            'baseurl'
        );

        foreach ($variables as $variable) {
            $file = public_path("/serverSetting/$variable.txt");
            $newVal = $request[$variable];
            file_put_contents($file, $newVal);
        }

        exec(public_path("/networkSetting/conf.sh"));
        return Redirect('/server-setting')->with(Session::flash('flash_message', 'اطلاعات سرور با موفقیت ویرایش شد.'));

    }


}
