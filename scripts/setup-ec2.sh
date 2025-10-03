#!/bin/bash

# EC2 Server Setup Script for MkDocs Deployment
# This script should be run on your EC2 instance to prepare it for deployment

set -e

# Variables (customize these for your setup)
NGINX_SITE_NAME="simtestlab"
WEB_ROOT="/var/www/simtestlab"
DOMAIN_NAME="your-domain.com"  # Replace with your actual domain

echo "🚀 Setting up EC2 instance for MkDocs deployment..."

# Update system packages
echo "📦 Updating system packages..."
sudo apt update && sudo apt upgrade -y

# Install nginx
echo "🌐 Installing Nginx..."
sudo apt install nginx -y

# Create web root directory
echo "📁 Creating web root directory..."
sudo mkdir -p $WEB_ROOT
sudo chown -R $USER:$USER $WEB_ROOT
sudo chmod -R 755 $WEB_ROOT

# Create nginx site configuration
echo "⚙️  Configuring Nginx..."
sudo tee /etc/nginx/sites-available/$NGINX_SITE_NAME > /dev/null <<EOF
server {
    listen 80;
    listen [::]:80;
    
    server_name $DOMAIN_NAME www.$DOMAIN_NAME;
    root $WEB_ROOT;
    index index.html index.htm;

    # Security headers
    add_header X-Frame-Options "SAMEORIGIN" always;
    add_header X-XSS-Protection "1; mode=block" always;
    add_header X-Content-Type-Options "nosniff" always;
    add_header Referrer-Policy "no-referrer-when-downgrade" always;
    add_header Content-Security-Policy "default-src 'self' http: https: data: blob: 'unsafe-inline'" always;

    # Gzip compression
    gzip on;
    gzip_vary on;
    gzip_min_length 1024;
    gzip_proxied expired no-cache no-store private must-revalidate auth;
    gzip_types text/plain text/css text/xml text/javascript application/x-javascript application/xml+rss;

    # Cache static assets
    location ~* \.(jpg|jpeg|png|gif|ico|css|js|pdf)$ {
        expires 1y;
        add_header Cache-Control "public, immutable";
    }

    # Main location block
    location / {
        try_files \$uri \$uri/ =404;
    }

    # Handle MkDocs search
    location ~* \.(json)$ {
        expires 1h;
        add_header Cache-Control "public";
    }

    # Security: deny access to hidden files
    location ~ /\. {
        deny all;
    }
}
EOF

# Enable the site
echo "🔗 Enabling Nginx site..."
sudo ln -sf /etc/nginx/sites-available/$NGINX_SITE_NAME /etc/nginx/sites-enabled/
sudo rm -f /etc/nginx/sites-enabled/default

# Test nginx configuration
echo "🧪 Testing Nginx configuration..."
sudo nginx -t

# Start and enable nginx
echo "▶️  Starting Nginx..."
sudo systemctl start nginx
sudo systemctl enable nginx

# Configure firewall
echo "🔥 Configuring firewall..."
sudo ufw allow 'Nginx Full'
sudo ufw allow OpenSSH
sudo ufw --force enable

# Install fail2ban for additional security
echo "🛡️  Installing fail2ban..."
sudo apt install fail2ban -y
sudo systemctl start fail2ban
sudo systemctl enable fail2ban

# Create SSL certificate directory (for future Let's Encrypt setup)
echo "🔐 Preparing for SSL..."
sudo mkdir -p /etc/letsencrypt

# Create a simple index.html as placeholder
echo "📄 Creating placeholder index.html..."
cat > $WEB_ROOT/index.html <<EOF
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Simtestlab - Coming Soon</title>
    <style>
        body { 
            font-family: Arial, sans-serif; 
            text-align: center; 
            margin: 50px; 
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            min-height: 80vh;
            display: flex;
            align-items: center;
            justify-content: center;
            flex-direction: column;
        }
        .container { background: rgba(255,255,255,0.1); padding: 40px; border-radius: 10px; }
        h1 { font-size: 3em; margin-bottom: 20px; }
        p { font-size: 1.2em; }
    </style>
</head>
<body>
    <div class="container">
        <h1>🚀 Simtestlab</h1>
        <p>Documentation site deployment in progress...</p>
        <p><em>This placeholder will be replaced by your MkDocs site</em></p>
    </div>
</body>
</html>
EOF

# Set proper permissions
sudo chown -R $USER:$USER $WEB_ROOT
sudo chmod -R 755 $WEB_ROOT

echo "✅ EC2 setup completed successfully!"
echo ""
echo "📋 Next steps:"
echo "1. Add your GitHub repository's public key to ~/.ssh/authorized_keys"
echo "2. Configure GitHub secrets with your EC2 details"
echo "3. Update the domain name in nginx configuration if needed"
echo "4. Consider setting up SSL with Let's Encrypt:"
echo "   sudo apt install certbot python3-certbot-nginx"
echo "   sudo certbot --nginx -d $DOMAIN_NAME -d www.$DOMAIN_NAME"
echo ""
echo "🌐 Your site will be available at: http://$DOMAIN_NAME"
echo "📁 Web root: $WEB_ROOT"
echo "⚙️  Nginx config: /etc/nginx/sites-available/$NGINX_SITE_NAME"