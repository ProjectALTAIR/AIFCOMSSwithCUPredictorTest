## If you have SELinux (Security-Enhanced Linux) enabled in your Linux installation, 
### you will need to do these additional steps:

In `/etc/apache2/sites-available/000-default.conf`
change the default "DocumentRoot"  from `/var/www` to `/var/www/html`

and within that same file, add the following:
        <Directory "/var/www/html">
            Options FollowSymLinks
            AllowOverride None
            Require all granted
        </Directory>

and in `etc/apache2/mods-available/php8.1.conf`
comment the last 5 lines, i.e. the lines:  
        <IfModule mod_userdir.c>
            <Directory /home/*/public_html>
                php_admin_flag engine Off
            </Directory>
        </IfModule>
