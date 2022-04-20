When it says ./, it's in C:\Program Files (x86)\Windows Kits\10\App Certification Kit
Make Appx:
./makeappx pack /v /h SHA256 /d "<directoryafterwindeployqt>" /p "<pathtofinalappxfile>"
Make Cert:
New-SelfSignedCertificate -Type Custom -Subject "CN=<name>, O=<name>, L=<city>, S=<state>, C=<country>" -KeyUsage DigitalSignature -FriendlyName "Cert for Mechanical Babulya" -TextExtension @("2.5.29.37={text}1.3.6.1.5.5.7.3.3", "2.5.29.19={text}") 
Save Cert:
(According to https://stackoverflow.com/questions/51498409/what-password-does-domain-protected-pfx-require)
$cert = @(Get-ChildItem -Path '<pathtocertthumbnail>')[0]
$certBytes = $cert.Export([System.Security.Cryptography.X509Certificates.X509ContentType]::Pfx)
[System.IO.File]::WriteAllBytes('<savepfxfilewithpath>', $certBytes)
Sign Appx:
 ./SignTool sign /fd "SHA256" /a /f "<locationof.pfxfile>" "locationof.appxfile"
Move certs according to this:
https://docs.microsoft.com/en-us/windows/msix/app-installer/troubleshoot-appinstaller-issues