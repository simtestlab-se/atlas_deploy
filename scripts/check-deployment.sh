#!/bin/bash

# Deployment Status and Health Check Script
# Run this on your EC2 instance to check deployment status

echo "🔍 Simtestlab Deployment Status Check"
echo "======================================="

# Check if nginx is running
echo -n "Nginx Status: "
if systemctl is-active --quiet nginx; then
    echo "✅ Running"
else
    echo "❌ Not running"
    echo "To start: sudo systemctl start nginx"
fi

# Check nginx configuration
echo -n "Nginx Config: "
if nginx -t 2>/dev/null; then
    echo "✅ Valid"
else
    echo "❌ Invalid"
    echo "Run 'sudo nginx -t' for details"
fi

# Check if site directory exists and has content
SITE_DIR="/var/www/simtestlab"
echo -n "Site Directory: "
if [ -d "$SITE_DIR" ] && [ "$(ls -A $SITE_DIR)" ]; then
    echo "✅ Exists with content"
    echo "   📁 Files: $(find $SITE_DIR -type f | wc -l)"
    echo "   📂 Size: $(du -sh $SITE_DIR | cut -f1)"
else
    echo "❌ Missing or empty"
    echo "   Expected location: $SITE_DIR"
fi

# Check site accessibility
echo -n "Local HTTP Test: "
if curl -s -o /dev/null -w "%{http_code}" localhost | grep -q "200"; then
    echo "✅ Responding (HTTP 200)"
else
    echo "❌ Not responding"
fi

# Check SSL certificate status (if exists)
if [ -f /etc/letsencrypt/live/*/fullchain.pem ]; then
    echo -n "SSL Certificate: "
    CERT_PATH=$(find /etc/letsencrypt/live -name "fullchain.pem" | head -1)
    if openssl x509 -checkend 2592000 -noout -in "$CERT_PATH" 2>/dev/null; then
        EXPIRY=$(openssl x509 -enddate -noout -in "$CERT_PATH" | cut -d= -f2)
        echo "✅ Valid until $EXPIRY"
    else
        echo "⚠️  Expires within 30 days or invalid"
    fi
else
    echo "SSL Certificate: ⚠️  Not configured"
fi

# Check recent deployment activity
echo ""
echo "📊 Recent Activity:"
echo "-------------------"
echo "Last 5 nginx access logs:"
if [ -f /var/log/nginx/simtestlab_access.log ]; then
    tail -5 /var/log/nginx/simtestlab_access.log 2>/dev/null || echo "No access logs found"
else
    tail -5 /var/log/nginx/access.log 2>/dev/null || echo "No access logs found"
fi

echo ""
echo "Last site file modification:"
if [ -d "$SITE_DIR" ]; then
    find $SITE_DIR -type f -printf '%TY-%Tm-%Td %TT %p\n' 2>/dev/null | sort -r | head -1 || echo "No files found"
else
    echo "Site directory not found"
fi

# System resource usage
echo ""
echo "💻 System Resources:"
echo "--------------------"
echo "Disk Usage:"
df -h / | tail -1 | awk '{print "   Root: " $3 "/" $2 " (" $5 " used)"}'
if [ -d "$SITE_DIR" ]; then
    du -sh $SITE_DIR | awk '{print "   Site: " $1}'
fi

echo "Memory Usage:"
free -h | grep "Mem:" | awk '{print "   RAM: " $3 "/" $2 " (" int($3/$2*100) "% used)"}'

echo "Load Average:"
uptime | awk -F'load average:' '{print "   " $2}'

echo ""
echo "🌐 Network Information:"
echo "-----------------------"
echo "Public IP: $(curl -s ifconfig.me 2>/dev/null || echo 'Unable to determine')"
echo "Listening ports:"
ss -tuln | grep ':80\|:443\|:22' | awk '{print "   " $1 " " $5}' || echo "   Unable to check ports"

# Check firewall status
echo ""
echo "🔥 Firewall Status:"
echo "-------------------"
if command -v ufw >/dev/null 2>&1; then
    ufw status | head -5
else
    echo "UFW not installed"
fi

echo ""
echo "🔚 Status check complete!"
echo ""
echo "💡 Quick commands:"
echo "   Restart nginx: sudo systemctl restart nginx"
echo "   View logs: sudo tail -f /var/log/nginx/error.log"
echo "   Update site: Check GitHub Actions deployment status"
echo "   SSL setup: sudo certbot --nginx -d yourdomain.com"